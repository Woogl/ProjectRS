#include "RsProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/Player/RsProjectilePredictionComponent.h"

FDetonationInfo::FDetonationInfo(bool bInDetonated, bool bInHasDirectImpactTarget, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, const FVector& InHitLocation, const FVector& InHitNormal)
	: bDetonated(bInDetonated), bHasDirectImpactTarget(bInHasDirectImpactTarget), OtherActor(InOtherActor), OtherComp(InOtherComp), HitLocation(InHitLocation), HitNormal(InHitNormal)
{
}

bool FDetonationInfo::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = true;
	bool bOutSuccessLocal = true;
	Ar << bDetonated;
	Ar << bHasDirectImpactTarget;
	Ar << OtherActor;
	Ar << OtherComp;
	HitLocation.NetSerialize(Ar, Map, bOutSuccessLocal);
	bOutSuccess &= bOutSuccessLocal;
	HitNormal.NetSerialize(Ar, Map, bOutSuccessLocal);
	bOutSuccess &= bOutSuccessLocal;
	return bOutSuccess;
}

ARsProjectile::ARsProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Root component performs collision detection against the environment and targets (pawns and damageable actors).
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARsProjectile::OnHitboxOverlapBegin);
	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->SetUseCCD(true);
	CollisionComp->bReceivesDecals = false;
	SetRootComponent(CollisionComp);

	// Set up our projectile movement.
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(CollisionComp);
	ProjectileMovement->bForceSubStepping = true; // Mitigates issues when fast-forwarding the projectile, especially when detonating on spawn.
	ProjectileMovement->MaxSimulationTimeStep = 0.0166;
	ProjectileMovement->MaxSimulationIterations = 16;
	ProjectileMovement->bInterpolationUseScopedMovement = false;
	ProjectileMovement->OnProjectileBounce.AddDynamic(this, &ARsProjectile::OnBounce);
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ARsProjectile::OnStop);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	InitialLifeSpan = 5.0f;

	// Networking.
	bReplicates = true;
	bReplicateProjectileMovement = false;
	NetPriority = 2.0f;
	SetMinNetUpdateFrequency(100.0f);

	bHasSpawnedFully = false;
	ProjectileId = INVALID_PROJECTILE_ID;
	bIsFakeProjectile = false;
	LinkedFakeProjectile = nullptr;
	LinkedAuthProjectile = nullptr;
	bCorrectFakeProjectilePositionOverTime = false;
	InitialProjectileError = 0.0f;
	bDetonated = false;
	bInOverlap = false;
	bFinishedResim = false;
	ImpactGameplayEffect = nullptr;

	ImpactEffectDirection = EEffectDirection::InProjectileDirection;
	AreaRadius = 0.0f;
	AreaOffset = FVector(0.0f);
	AreaGameplayEffect = nullptr;
	bSkipAreaEffectForImpactTarget = false;
	bPredictFX = true;
	LifeSpanAfterDetonation = 0.0f;
	bTriggeredFX = false;
	PlayerController = nullptr;
	TearOffDelay = 0.1f;
	MinLifetime = 0.05f;
}

void ARsProjectile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SpawnTransform = Transform;
}

void ARsProjectile::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	/* Cache our player controller in case our instigator dies during our projectile's lifetime. Replicate it because
	 * the server might need it before BeginPlay (if we detonate on spawn), but the clients won't have it yet. */
	if (GetInstigator())
	{
		PlayerController = GetInstigatorController<APlayerController>();
	}

	// Set up rendering for this projectile's meshes.
	TArray<UMeshComponent*> MeshComponents;
	GetComponents<UMeshComponent>(MeshComponents);
	for (UMeshComponent* MeshComp : MeshComponents)
	{
		MeshComp->bUseAsOccluder = false;
		MeshComp->SetCastShadow(false);
	}
}

void ARsProjectile::BeginPlay()
{
	Super::BeginPlay();

	/* Predicted projectiles must have an instigating player controller. We need it so we can retrieve their ping for
	 * prediction logic. We don't need a PC for non-predicted projectiles (or projectiles spawned on the server without
	 * an instigator, like environmental hazards), since the prediction logic will be skipped. */
	if ((!HasAuthority() && ProjectileId != INVALID_PROJECTILE_ID) || bIsFakeProjectile)
	{
		if (!ensureAlwaysMsgf(GetInstigator() && PlayerController.IsValid(), TEXT("Instigating player controller could not be found for predicted projectile projectile (%s). Failed to find player controller with instigator (%s). Predicted projectiles must be spawned with an instigator with a valid player controller."), *GetName(), *GetNameSafe(GetInstigator())))
		{
			Destroy();
			return;
		}
	}

	/** If this projectile has a valid instigator, we assume it was spawned by a player or AI, and thus want to apply
	 * gameplay effects using that instigator's ASC. To retrieve that ASC, we also need a valid owner. */
	if (GetInstigator())
	{
		if (!ensureAlwaysMsgf(IsValid(Owner), TEXT("Projectile spawned by (%s) does not have a valid owner. Projectiles spawned by players or AI must have an owner so their ASC can be found to apply gameplay effects. Otherwise, GEs will be applied without an instigator."), *GetInstigator()->GetName()))
		{
			Destroy();
			return;
		}
	}

	/* Projectiles can't be spawned directly on clients. A client-authoritative projectile can't perform gameplay logic,
	 * so it would just be cosmetic, which is a waste of the projectile logic overhead. Just create a standalone
	 * projectile class derived from AActor instead. */
	if (!ensureAlwaysMsgf(!(GetNetMode() == NM_Client && HasAuthority() && !bIsFakeProjectile), TEXT("Spawned projectile (%s) directly on client, which is not allowed. Projectiles should be spawned on the server to be replicated to clients, or should be spawned predictively with the \"Spawn Predicted Projectile\" ability task."), *GetName()))
	{
		Destroy();
		return;
	}

	// Initialize the replicated authoritative projectile once it's replicated to the owning client.
	if (!HasAuthority() && (ProjectileId != INVALID_PROJECTILE_ID))
	{
		/* Fast-forward the projectile to match where it is on the server to make up for replication time. We only want
		 * to do this if we're going to be correcting the fake projectile's position, because we want to correct it to
		 * where it is on the server. */
		float CatchupTickDelta = (PlayerController->PlayerState) ? (0.0005f * PlayerController->PlayerState->ExactPing) : 0.0f;
		if (CatchupTickDelta > 0.0f && bCorrectFakeProjectilePositionOverTime)
		{
			CatchupTick(CatchupTickDelta);
		}

		UE_LOG(RsAbilityLog, Verbose, TEXT("(%i:%i:%i) (ID: %i): Successfully replicated authoritative projectile (%s) to client. Forwarding (%fms) to make up for replication time with ping (%fms)."), FDateTime::UtcNow().GetMinute(), FDateTime::UtcNow().GetSecond(), FDateTime::UtcNow().GetMillisecond(), ProjectileId, *GetName(), CatchupTickDelta * 1000.0f, PlayerController->PlayerState->ExactPing);

		// Link to the associated fake projectile.
		if (URsProjectilePredictionComponent* PredComp = PlayerController->FindComponentByClass<URsProjectilePredictionComponent>())
		{
			if (ensureAlwaysMsgf(PredComp->FakeProjectiles.Contains(ProjectileId), TEXT("Client-side authoritative projectile (%s) failed to find corresponding fake projectile with ID (%i)."), *GetNameSafe(this), ProjectileId))
			{
				LinkFakeProjectile(PredComp->FakeProjectiles[ProjectileId]);
				PredComp->FakeProjectiles.Remove(ProjectileId);
			}
		}
	}

	/* On simulated proxies without a fake projectile (i.e. non-owning simulated proxies), projectiles will appear a
	 * noticeable distance ahead of where they spawned (or sometimes not at all) due to latency and forward prediction.
	 * To prevent this, we rewind the projectiles back to their spawn position and resimulate them to show their full
	 * lifetime. We also do this for projectiles (on all machines) that aren't spawned predictively. */
	if ((GetLocalRole() == ROLE_SimulatedProxy) && (ProjectileId == INVALID_PROJECTILE_ID))
	{
		/** When projectiles detonate on spawn, we rewind and resimulate them using FinishedResimulationTimer, meaning
		 * we'll have already done this and don't need to do it again.
		 * @see OnRep_DetonationInfo */
		if (!bFinishedResim && !GetWorldTimerManager().IsTimerActive(FinishedResimulationTimer))
		{
			SetActorTransform(SpawnTransform);
		}
	}

#if WITH_EDITOR
	// if (const UGASDeveloperSettings* DevSettings = GetDefault<UGASDeveloperSettings>())
	// {
	// 	if (DevSettings->ProjectileDebugMode != None)
	// 	{
	// 		// Enable debug drawing for the server's projectile.
	// 		if (HasAuthority() && !bIsFakeProjectile)
	// 		{
	// 			/* It's difficult to synchronize the server and client's draws, so instead we draw the server's
	// 			 * projectile more frequently, to show a (somewhat) continuous depiction of where the server is. */
	// 			const float ServerDrawScale = (20.0f / DevSettings->DrawFrequency);
	// 			constexpr float MinRate = 0.01f;
	// 			const float DrawRate = FMath::Max((ServerDrawScale / ProjectileMovement->InitialSpeed), MinRate);
	// 			GetWorldTimerManager().SetTimer(DrawDebugTimer, FTimerDelegate::CreateUObject(this, &ARsProjectile::DrawDebug), DrawRate, true);
	// 		}
	// 		else
	// 		{
	// 			const float ClientDrawScale = (100.0f / DevSettings->DrawFrequency);
	// 			constexpr float MinRate = 0.02f;
	// 			const float DrawRate = FMath::Max((ClientDrawScale / ProjectileMovement->InitialSpeed), MinRate);
	//
	// 			/* Enable debug drawing for the client's auth projectile. Once linked, this also performs drawing for
	// 			 * the fake projectile, so each draw is synchronized (so we can see their difference at each step). */
	// 			if (IsValid(LinkedFakeProjectile))
	// 			{
	// 				GetWorldTimerManager().SetTimer(DrawDebugTimer, FTimerDelegate::CreateUObject(this, &ARsProjectile::DrawDebug), DrawRate, true);
	//
	// 				// Take over drawing for the fake projectile.
	// 				GetWorldTimerManager().ClearTimer(LinkedFakeProjectile->DrawDebugTimer);
	// 			}
	// 			/* If desired, enable debug drawing for the fake projectile until the authoritative projectile is
	// 			 * linked, at which point the authoritative projectile will take over drawing. */
	// 			else if (bIsFakeProjectile && !DevSettings->bWaitForLinkage)
	// 			{
	// 				GetWorldTimerManager().SetTimer(DrawDebugTimer, FTimerDelegate::CreateUObject(this, &ARsProjectile::DrawDebug), DrawRate, true);
	// 			}
	// 		}
	//
	// 	}
	//
	// 	// Draw the initial position of each projectile if desired.
	// 	if (DevSettings->bDrawSpawnPosition)
	// 	{
	// 		DrawProjectileStep(GetDebugColor(), false, true);
	// 	}
	// }
#endif // WITH_EDITOR

	bHasSpawnedFully = true;
}

void ARsProjectile::CatchupTick(float CatchupTickDelta)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->TickComponent(CatchupTickDelta, LEVELTICK_All, nullptr);
	}
}

void ARsProjectile::SetLifeSpan(float InLifespan)
{
	InitialLifeSpan = InLifespan;
	if (InLifespan > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_LifeSpanExpired, this, &AActor::LifeSpanExpired, InLifespan);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_LifeSpanExpired);
	}
}

void ARsProjectile::TornOff()
{
	/* The projectile is torn off when it detonates on the server, which forces detonation on clients if it hasn't
	 * already. */
	if (!bDetonated)
	{
		if (DetonationInfo.bDetonated)
		{
			// Server tore off a non-owning simulated proxy before it finished its local resimulation.
			if (ProjectileId == INVALID_PROJECTILE_ID)
			{
				UE_LOG(RsAbilityLog, Warning, TEXT("Projectile (%s) on non-owning simulated proxy was torn off before it finished its resimulation. Is MinLifetime less than TearOffDelay? Detonating early with replicated detonation information..."), *GetName());

				DetonateWithDetonationInfo();
			}
			/*
			 * Predicting client's version of the authoritative projectile hasn't detonated yet. This happens when the
			 * projectile detonates on spawn, but the detonation is rejected because the projectile hasn't been fully
			 * initialized yet (@see ARsProjectile::Detonate), in which case, we try to detonate again once being torn
			 * off.
			 *
			 * This can also theoretically happen if the client's version of the authoritative projectile somehow misses
			 * whatever it hit on the server, though this is extremely rare.
			 */
			else
			{
				DetonateWithDetonationInfo();
			}
		}
		// Server tore off a non-owning simulated proxy without replicating the detonation.
		else if (ProjectileId == INVALID_PROJECTILE_ID)
		{
			checkf(1, TEXT("Projectile (%s) on non-owning simulated proxy was torn off, but it never received any detonation information, meaning the server tore off the projectile without detonating it."), *GetName());
		}
	}
}

void ARsProjectile::ShutDown()
{
#if WITH_EDITOR
	// Stop debug drawing when this projectile shuts down.
	GetWorldTimerManager().ClearTimer(DrawDebugTimer);
#endif // WITH_EDITOR

	/* If we want to keep this projectile visible after its detonation (i.e. for post-detonation FX, like a landing
	 * animation), make sure it's not hidden. E.g. if we're not predicting FX, we need to unhide the authoritative
	 * projectile when it detonates, since authoritative projectiles are hidden by default on the owning client. */
	if (bTriggeredFX && (LifeSpanAfterDetonation > 0.0f))
	{
		SetActorHiddenInGame(false);
		TArray<USceneComponent*> Components;
		GetComponents<USceneComponent>(Components);
		for (int32 i = 0; i < Components.Num(); i++)
		{
			Components[i]->SetVisibility(true);
		}
	}

	if (!IsPendingKillPending())
	{
		// Disable movement & collision.
		SetActorEnableCollision(false);
		ProjectileMovement->SetActive(false);

		// Make sure we keep the projectile alive long enough to finish replicating.
		float NewLifeSpan = FMath::Max(LifeSpanAfterDetonation, 0.4f);

		// If we want to kill this projectile before it can be safely destroyed, disable and hide it until it can be.
		if (LifeSpanAfterDetonation == 0.0f)
		{
			DisableAndHide();
		}
		/* If the fake projectile isn't predicting FX, it should always be disabled and hidden after detonation. We'll
		 * be switching to the authoritative projectile for FX and post-detonation effects instead. We still want to
		 * keep the fake projectile alive for a bit, so the authoritative projectile has opportunity to check for missed
		 * predictions. */
		else if (bIsFakeProjectile && !bPredictFX)
		{
			DisableAndHide();
		}
		else if (LifeSpanAfterDetonation < NewLifeSpan)
		{
			GetWorldTimerManager().SetTimer(ShutDownTimer, FTimerDelegate::CreateUObject(this, &ARsProjectile::DisableAndHide), LifeSpanAfterDetonation, false);
		}

		SetLifeSpan(NewLifeSpan);

		K2_ShutDown();
	}
}

void ARsProjectile::Destroyed()
{
#if WITH_EDITOR
	/* If this projectile never detonated, draw its final position if desired, since it won't get a chance to in
	 * Detonate(). This usually happens if the fake projectile misses its target and gets replaced by the authoritative
	 * projectile. */
	// if (!bDetonated)
	// {
	// 	if (const UGASDeveloperSettings* DevSettings = GetDefault<UGASDeveloperSettings>())
	// 	{
	// 		if (DevSettings->bDrawFinalPosition && GetWorld()->IsGameWorld())
	// 		{
	// 			DrawProjectileStep(GetDebugColor(), false, true);
	// 		}
	// 	}
	// }
#endif // WITH_EDITOR

	// Redundancy for safety. Should never happen.
	if (LinkedFakeProjectile)
	{
		LinkedFakeProjectile->Destroy();
	}

	GetWorldTimerManager().ClearAllTimersForObject(this);

	Super::Destroyed();
}

void ARsProjectile::DisableAndHide()
{
	ForEachComponent(false, [](UActorComponent* InComponent)
	{
		// Stop VFX.
		if (UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(InComponent))
		{
			// Let VFX tick one last time before stopping.
			NiagaraComp->SetForceSolo(true);
			NiagaraComp->TickComponent(0.0f, LEVELTICK_All, nullptr);
			NiagaraComp->Deactivate();
			NiagaraComp->SetAutoDestroy(true);
		}
		// Stop looping audio (i.e. ambient sounds).
		else if (UAudioComponent* AudioComp = Cast<UAudioComponent>(InComponent))
		{
			if (AudioComp->GetSound() != nullptr && !AudioComp->GetSound()->IsOneShot())
			{
				AudioComp->Stop();
			}
		}
		// Hide scene components.
		else if (USceneComponent* SceneComp = Cast<USceneComponent>(InComponent))
		{
			SceneComp->SetHiddenInGame(true);
			SceneComp->SetVisibility(false);
		}
	});
}

void ARsProjectile::InitFakeProjectile(APlayerController* OwningPlayer, uint32 InProjectileId)
{
	if (InProjectileId == INVALID_PROJECTILE_ID)
	{
		return;
	}

	bIsFakeProjectile = true;
	if (OwningPlayer)
	{
		if (URsProjectilePredictionComponent* PredComp = OwningPlayer->FindComponentByClass<URsProjectilePredictionComponent>())
		{
			PredComp->FakeProjectiles.Add(InProjectileId, this);
		}
	}
}

void ARsProjectile::LinkFakeProjectile(ARsProjectile* InFakeProjectile)
{
	LinkedFakeProjectile = InFakeProjectile;
	InFakeProjectile->LinkedAuthProjectile = this;

	InitialProjectileError = (GetActorLocation() - LinkedFakeProjectile->GetActorLocation()).Size();
	if (((GetActorLocation() - LinkedFakeProjectile->GetActorLocation()) | LinkedFakeProjectile->GetVelocity()) > 0.f)
	{
		InitialProjectileError *= -1.0f;
	}

	UE_LOG(RsAbilityLog, Verbose, TEXT("Fake projectile (%s) linked. Error Margin: (%f)."), *GetNameSafe(LinkedFakeProjectile), InitialProjectileError);

	SetActorHiddenInGame(true);
	TArray<USceneComponent*> Components;
	GetComponents<USceneComponent>(Components);
	for (int32 i = 0; i < Components.Num(); i++)
	{
		Components[i]->SetVisibility(false);
	}
}

void ARsProjectile::SwitchToRealProjectile()
{
	// Unhide the authoritative projectile.
	SetActorHiddenInGame(false);
	TArray<USceneComponent*> Components;
	GetComponents<USceneComponent>(Components);
	for (int32 i = 0; i < Components.Num(); i++)
	{
		Components[i]->SetVisibility(true);
	}

	// Immediately destroy the fake projectile; it's no longer valid for prediction.
	if (LinkedFakeProjectile)
	{
		LinkedFakeProjectile->Destroy();
		LinkedFakeProjectile = nullptr;
	}
}

void ARsProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCorrectFakeProjectilePositionOverTime)
	{
		CorrectionLerpTick(DeltaSeconds);
	}
}

void ARsProjectile::CorrectionLerpTick(float DeltaTime)
{
	if (LinkedFakeProjectile && !LinkedFakeProjectile->bDetonated && !LinkedFakeProjectile->IsPendingKillPending() && ProjectileMovement)
	{
		if (DeltaTime > UE_SMALL_NUMBER)
		{
			FVector Current = LinkedFakeProjectile->GetActorLocation();
			FVector Target = GetActorLocation();

			/* We choose to lerp the projectile over a duration of (2000 / its initial speed). E.g. a projectile that
			 * travels at 1000m/s will be lerped over 2.0s. I don't think this is something designers should worry
			 * about, but we could expose it for configuration if we want to. */
			const float TargetLerpTime = 2000.0f / ProjectileMovement->InitialSpeed;
			const float StepDistance = (InitialProjectileError / TargetLerpTime);

			FVector NewLoc = FMath::VInterpConstantTo(Current, Target, DeltaTime, StepDistance);

			// We can just hack into RepMovement to safely send the location update.
			FRepMovement RepMovement(LinkedFakeProjectile->GetReplicatedMovement());
			RepMovement.Location = NewLoc;
			RepMovement.Rotation = LinkedFakeProjectile->GetActorRotation();
			LinkedFakeProjectile->SetReplicatedMovement(RepMovement);
			LinkedFakeProjectile->PostNetReceiveLocationAndRotation();

#if WITH_EDITOR
			// // Debugging.
			// if (GetDefault<UGASDeveloperSettings>()->bLogCorrection)
			// {
			// 	UE_LOG(RsAbilityLog, Log, TEXT("(%f): Moved fake projectile (%fm). Currently (%fm) from target."), GetWorld()->GetTimeSeconds(), FVector::Dist(NewLoc, Current), FVector::Dist(Target, NewLoc));
			// }
#endif // WITH_EDITOR
		}
	}
}

void ARsProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	// Trigger FX on owning client's auth projectile if we aren't predicting them.
	const bool bIsAuthOnOwner = ((!HasAuthority() || GetTearOff()) && (ProjectileId != INVALID_PROJECTILE_ID));
	if (!bIsAuthOnOwner || !bPredictFX)
	{
		/* Trigger FX on fake projectile if we ARE predicting them, or if we don't have an auth projectile yet, since
		 * our auth projectile probably won't finish initializing before it was supposed to bounce. */
		if (!bIsFakeProjectile || bPredictFX || !LinkedAuthProjectile)
		{
			BounceFX.ExecuteEffects(this, ImpactResult.ImpactPoint, ImpactResult.ImpactNormal.Rotation(), ImpactResult.GetComponent());
		}
	}
}

void ARsProjectile::OnHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	// Never detonate against the actor that fired this projectile (usually a player character).
	if (OtherActor == GetInstigator())
	{
		return;
	}

	if (!bInOverlap)
	{
		// Re-entrance guard.
		TGuardValue<bool> OverlapGuard(bInOverlap, true);

		// Check for line of sight.
		const FVector ImpactPoint = (bFromSweep ? FVector(SweepResult.ImpactPoint) : (OtherComp != nullptr ? OtherComp->GetComponentLocation() : OtherActor->GetActorLocation()));
		FCollisionQueryParams Params(FName(TEXT("HitboxOverlapTrace")), true, this);
		Params.AddIgnoredActor(OtherActor);
		if (GetWorld()->LineTraceTestByChannel(ImpactPoint, GetActorLocation(), ECC_Visibility, Params))
		{
			return;
		}

		// Get a hit result to apply effects (VFX locations, damage direction, etc.).
		FHitResult Hit;

		// Use the sweep if we have one.
		if (bFromSweep)
		{
			Hit = SweepResult;
		}
		else
		{
			// Try to generate a hit by sweeping the hitbox component.
			OtherComp->SweepComponent(Hit, GetActorLocation() - (GetVelocity() * 10.0f), GetActorLocation() + GetVelocity(), CollisionComp->GetComponentQuat(), CollisionComp->GetCollisionShape(), CollisionComp->bTraceComplexOnMove);

			// Try to generate a hit by just tracing against the component. 
			if (Hit.GetActor() != OtherActor)
			{
				OtherComp->LineTraceComponent(Hit, GetActorLocation() - (GetVelocity() * 10.0f), GetActorLocation() + GetVelocity(), FCollisionQueryParams(GetClass()->GetFName(), false, this));
			}

			// If we STILL fail, we'll just construct the hit manually. This is fine, since it's just for visuals.
			if (Hit.GetActor() != OtherActor)
			{
				const FVector Normal = (ImpactPoint - GetActorLocation()).GetSafeNormal();
				Hit = FHitResult(OtherActor, OtherComp, ImpactPoint, Normal);
			}
		}

		// Detonate this projectile against the hit actor.
		Detonate(true, OtherActor, OtherComp, Hit.ImpactPoint, Hit.ImpactNormal);
	}
}

void ARsProjectile::OnStop(const FHitResult& Hit)
{
	// Projectiles only stop when hitting the environment, so we detonate without a direct hit target.
	Detonate(false, Hit.GetActor(), Hit.GetComponent(), Hit.Location, Hit.Normal);
}

void ARsProjectile::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	if (bReplicateProjectileMovement && IsServerProjectile())
	{
		if (RootComponent && RootComponent->GetAttachParent())
		{
			Super::PreReplication(ChangedPropertyTracker);
		}
		else
		{
			GatherCurrentMovement();
		}
	}
}

void ARsProjectile::GatherCurrentMovement()
{
	if (RootComponent)
	{
		// If we're attached to something, use AttachmentReplication instead, which our super call can handle.
		if (RootComponent->GetAttachParent())
		{
			Super::GatherCurrentMovement();
		}
		else
		{
			ReplicatedProjectileMovement.Location = RootComponent->GetComponentLocation();
			ReplicatedProjectileMovement.Rotation = RootComponent->GetComponentRotation();
			ReplicatedProjectileMovement.LinearVelocity = GetVelocity();

			MARK_PROPERTY_DIRTY_FROM_NAME(ARsProjectile, ReplicatedProjectileMovement, this);
		}
	}
}

void ARsProjectile::PostNetReceiveVelocity(const FVector& NewVelocity)
{
	ProjectileMovement->Velocity = NewVelocity;
}

FVector ARsProjectile::GetVelocity() const
{
	if (RootComponent != nullptr && (RootComponent->IsSimulatingPhysics() || ProjectileMovement == nullptr))
	{
		return GetRootComponent()->GetComponentVelocity();
	}
	else
	{
		return ProjectileMovement->Velocity;
	}
}

void ARsProjectile::OnRep_ReplicatedMovement()
{
	const FRepMovement& LocalRepMovement = GetReplicatedMovement();

	if (RootComponent)
	{
		/* Sync physics sim to match server. bRepPhysics should always be false for projectiles, so this shouldn't
		 * really be necessary. */
		if (RootComponent->IsSimulatingPhysics() != LocalRepMovement.bRepPhysics)
		{
			SyncReplicatedPhysicsSimulation();
		}

		RootComponent->OnReceiveReplicatedState(LocalRepMovement.Location, LocalRepMovement.Rotation.Quaternion(), LocalRepMovement.LinearVelocity, LocalRepMovement.AngularVelocity);

		if (!RootComponent->GetAttachParent())
		{
			/* Replicate the authoritative projectile's final position to the predicting client to make sure their
			 * fake projectile detonated in the correct position and to make sure their version of the authoritative
			 * projectile detonates in the correct position (for non-predicted FX and to check for missed
			 * predictions). */
			if (GetLocalRole() == ROLE_SimulatedProxy)
			{
				/* Don't replicate movement to non-owning simulated proxies. Due to their local resimulation, they'll
				 * intentionally be slightly behind the authoritative projectile. */
				if (ProjectileId != INVALID_PROJECTILE_ID)
				{
					PostNetReceiveVelocity(LocalRepMovement.LinearVelocity);
					PostNetReceiveLocationAndRotation();
				}
			}
		}
	}
}

void ARsProjectile::OnRep_RepProjectileMovement()
{
	if (GetLocalRole() == ROLE_SimulatedProxy)
	{
		FRepMovement NewMovement(GetReplicatedMovement());
		NewMovement.Location = ReplicatedProjectileMovement.Location;
		NewMovement.Rotation = ReplicatedProjectileMovement.Rotation;
		NewMovement.LinearVelocity = ReplicatedProjectileMovement.LinearVelocity;
		NewMovement.AngularVelocity = FVector(0.f);
		NewMovement.bSimulatedPhysicSleep = false;
		NewMovement.bRepPhysics = false;
		SetReplicatedMovement(NewMovement);

		OnRep_ReplicatedMovement();
	}
}

void ARsProjectile::Detonate(bool bHasDirectImpactTarget, AActor* OtherActor, UPrimitiveComponent* OtherComp, const FVector& HitLocation, const FVector& HitNormal)
{
	/** Never detonate non-owning simulated proxies until they've finished their local resimulation. */
	if (!IsServerProjectile() && ProjectileId == INVALID_PROJECTILE_ID && !bFinishedResim)
	{
		return;
	}

	/* Replicated authoritative projectiles aren't allowed to detonate before they finish initializing (i.e. on spawn),
	 * because we can't check for missed predictions yet. Instead, we wait until we get torn off, then try to detonate
	 * again. */
	if (!bDetonated && (HasAuthority() || bHasSpawnedFully))
	{
		UE_LOG(RsAbilityLog, VeryVerbose, TEXT("%s projectile (%s) hit actor (%s) on (%s)."),
			*FString(bIsFakeProjectile ? "Fake" : "Authoritative"), *GetName(), *GetNameSafe(OtherActor), *FString(IsServerProjectile() ? "server" : (ProjectileId == INVALID_PROJECTILE_ID ? "remote client" : "owning client")));

		/* Don't trigger FX with the owning client's authoritative projectile if the fake projectile is allowed to
		 * predict them, unless the fake projectile had a missed prediction that needs to be corrected. */
		const bool bIsAuthOnOwner = ((!HasAuthority() || GetTearOff()) && (ProjectileId != INVALID_PROJECTILE_ID));
		if (!bIsAuthOnOwner || ShouldAuthProjDetonateOnOwner(true))
		{
			// Don't trigger FX with fake projectiles if they don't predict them.
			if (!bIsFakeProjectile || bPredictFX)
			{
				bTriggeredFX = true;

				UE_LOG(RsAbilityLog, VeryVerbose, TEXT("\t\t... FX triggered."));

				const FVector Normal = (bHasDirectImpactTarget ? (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal() : HitNormal);

				// Always spawn detonation FX. Don't spawn decals if we hit a target (e.g. a player character).
				DetonationFX.ExecuteEffects(this, HitLocation, Normal.Rotation(), OtherComp, bHasDirectImpactTarget);

				// Spawn missed impact FX if we didn't directly hit a target (i.e. detonated against the environment).
				if (!bHasDirectImpactTarget)
				{
					MissedImpactFX.ExecuteEffects(this, HitLocation, Normal.Rotation(), OtherComp);
				}

				/* Forward event to BP. We only do this when FX are triggered to ensure each client calls this exactly
				 * once per projectile (with the exception of missed predictions). */
				OnDetonate(bHasDirectImpactTarget, OtherActor, OtherComp, HitLocation, Normal);
			}

			/* If our authoritative projectile doesn't detonate soon, that means we predicted wrong and hit
			 * something we shouldn't have. If this happens, switch to our authoritative projectile in an effort to
			 * reconcile. */
			if (bIsFakeProjectile)
			{
				/* TearOffDelay is used here because when authoritative projectiles detonate on spawn, their detonation
				 * on clients will be delayed by TearOffDelay to ensure they've been fully initialized and have had
				 * enough time to initially replicate. */
				GetWorldTimerManager().SetTimer(SwitchToAuthTimer, FTimerDelegate::CreateWeakLambda(this, [this]()
				{
					if (LinkedAuthProjectile)
					{
						if (!LinkedAuthProjectile->bDetonated)
						{
							UE_LOG(RsAbilityLog, Warning, TEXT("Missed prediction: Fake projectile (%s) detonated against something it shouldn't have. Reconciling by switching to real projectile..."), *GetNameSafe(this));
							LinkedAuthProjectile->SwitchToRealProjectile();
						}
					}
				}), (0.001 * (PlayerController->PlayerState->ExactPing + 60.0f)) + TearOffDelay, false);
			}
		}

		// Apply gameplay effects on the server.
		if (IsServerProjectile())
		{
			// If there was a direct impact on a target, apply the impact GE to them.
			if (ImpactGameplayEffect && bHasDirectImpactTarget && OtherActor)
			{
				const FVector Normal =  (ImpactEffectDirection == EEffectDirection::InProjectileDirection) ? GetActorRotation().Vector() :
										(ImpactEffectDirection == EEffectDirection::InVelocityDirection) ? ProjectileMovement->Velocity.GetSafeNormal() :
										/* FromProjectilePosition */ (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				FHitResult Hit = FHitResult(OtherActor, OtherComp, HitLocation, Normal);
				Hit.bBlockingHit = true;
				ApplyEffectToTarget(ImpactGameplayEffect, OtherActor, Hit);
			}

			// Apply the AOE effect to nearby actors, excluding the impact target.
			if ((AreaRadius > 0.0f) && AreaGameplayEffect)
			{
				TArray<FOverlapResult> OverlapResults;
				FVector Origin = GetAreaOfEffectOrigin();
				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(this);
				GetWorld()->OverlapMultiByChannel(
					OverlapResults,
					Origin,
					FQuat(),
					ECollisionChannel::ECC_WorldDynamic,
					FCollisionShape::MakeSphere(AreaRadius),
					QueryParams
				);

				for (const FOverlapResult& Overlap : OverlapResults)
				{
					if (AActor* ItTarget = Overlap.GetActor())
					{
						// Skip the impact target, if desired.
						if (bSkipAreaEffectForImpactTarget && (ItTarget == OtherActor))
						{
							continue;
						}

						// Check line of sight.
						FVector ImpactPoint = Overlap.OverlapObjectHandle.GetLocation();
						FCollisionQueryParams Params(FName(TEXT("AOEOverlapTrace")), true, this);
						Params.AddIgnoredActor(ItTarget);
						if (GetWorld()->LineTraceTestByChannel(ImpactPoint, Origin, ECC_Visibility, Params))
						{
							continue;
						}

						// Apply the AOE effect. (We use the actor's location for the direction, not the overlap point).
						FHitResult OverlapHit = FHitResult(ItTarget, Overlap.GetComponent(), ImpactPoint, (ItTarget->GetActorLocation() - Origin).GetSafeNormal());
						OverlapHit.bBlockingHit = true;
						ApplyEffectToTarget(AreaGameplayEffect, ItTarget, OverlapHit);
					}
				}
			}

			// Send detonation info to simulated proxies to use when they finish their local resimulation.
			DetonationInfo = FDetonationInfo(true, bHasDirectImpactTarget, OtherActor, OtherComp, HitLocation, HitNormal);
			MARK_PROPERTY_DIRTY_FROM_NAME(ARsProjectile, DetonationInfo, this);

			// Enable movement replication so the detonation is in the correct location for clients.
			bReplicateProjectileMovement = true;

			/** Tear off the server's projectile to trigger TornOff on all clients, which will force them to
			 * detonate if they haven't already. We're basically using this like an RPC that replicates the
			 * detonation event to clients, but waits until after they're replicated and initialized. Delay this if
			 * the projectile hasn't had enough time to initially replicate yet. */
			if (GetGameTimeSinceCreation() > TearOffDelay)
			{
				GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::TearOff));
			}
			else
			{
				GetWorldTimerManager().SetTimer(TearOffTimer, FTimerDelegate::CreateUObject(this, &ThisClass::TearOff), TearOffDelay, false);
			}
		}

		/* If the authoritative projectile triggered FX when it shouldn't have, that means there was a missed prediction
		 * that caused us to switch to the authoritative projectile. If this happens, we need to destroy the fake one
		 * (if it hasn't been destroyed already), since it's no longer valid for predicting. This is usually only 
		 * necessary when projectiles have post-detonation effects (i.e. LifeSpanAfterDetonation > 0). Otherwise, the
		 * fake projectile will likely have already been hidden or destroyed. */
		if (LinkedFakeProjectile && bTriggeredFX && bPredictFX)
		{
			UE_LOG(RsAbilityLog, VeryVerbose, TEXT("\t\t... Fake projectile (%s) destroyed due to missed prediction."), *LinkedFakeProjectile->GetName());

			LinkedFakeProjectile->Destroy();
			LinkedFakeProjectile = nullptr;
		}

		bDetonated = true;

		// Clean up and destroy this projectile.
		ShutDown();

#if WITH_EDITOR
		// Draw the projectiles' final position if desired.
		// if (GetDefault<UGASDeveloperSettings>()->bDrawFinalPosition)
		// {
		// 	DrawProjectileStep(GetDebugColor(), false, true);
		// 	DrawDetonationInfo(HitLocation, (bHasDirectImpactTarget ? (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal() : HitNormal));
		// }
#endif // WITH_EDITOR
	}
}

bool ARsProjectile::ShouldAuthProjDetonateOnOwner(bool bLog) const
{
	// If we aren't predicting FX, the authoritative projectile should trigger FX instead of the fake projectile.
	if (!bPredictFX)
	{
		return true;
	}

	if (LinkedFakeProjectile)
	{
		/*
		 * If the fake projectile hasn't detonated yet, we should destroy it (which is done at the end of Detonate), and
		 * switch to the authoritative projectile. This can occur if the fake projectile missed the correct target, or
		 * if the authoritative projectile was slightly ahead of the fake one, because we forward-predicted a little too
		 * far due to an inaccurate ping estimate.
		 *
		 * We could also handle this by correcting the fake projectile's position and forcing its detonation, but we
		 * choose to use the authoritative projectile instead because it's a cleaner solution; there's no point in using
		 * the fake (i.e. predicted) projectile if it's not actually predicting anything anymore. Plus, there will be a
		 * visual discrepancy either way (the fake projectile disappearing and the real one appearing, or the fake
		 * projectile teleporting).
		 */
		if (!LinkedFakeProjectile->bDetonated)
		{
			if (bLog)
			{
				UE_LOG(RsAbilityLog, Warning, TEXT("Missed prediction: Fake projectile (%s) missed its detonation. Reconciling by destroying the fake projectile and using real projectile's detonation..."), *GetNameSafe(LinkedFakeProjectile));
			}

			return true;
		}

		/* If the fake projectile already detonated, but did so inaccurately, we should correct it by replaying FX
		 * in the correct place with the authoritative projectile. */
		if (LinkedFakeProjectile->bDetonated)
		{
			constexpr float MaxFinalPositionDistance = 100.0f; // Centimeters
			if ((LinkedFakeProjectile->GetActorLocation() - GetActorLocation()).Size() > MaxFinalPositionDistance)
			{
				if (bLog)
				{
					UE_LOG(RsAbilityLog, Warning, TEXT("Missed prediction: Fake projectile (%s) detonated too early and/or in the wrong location (Error Margin: %fm). Reconciling by resimulating the detonation with the real projectile..."), *GetNameSafe(LinkedFakeProjectile), FVector::Dist(LinkedFakeProjectile->GetActorLocation(), GetActorLocation()) * 0.01f);
				}

				return true;
			}
		}
	}

	/*
	 * If we don't have a fake projectile at this point, that means either the fake projectile detonated so early
	 * that it's already been destroyed, or the authoritative projectile detonated so early that we haven't linked
	 * yet.
	 *
	 * For the first case, if we've already been fully initialized and still don't have a linked projectile, that
	 * means it detonated so long ago that it was already destroyed. The fake projectile will have already caught
	 * this (using the SwitchToAuthTimer timer) and switched to using this authoritative projectile on the client.
	 * All we have to do at this point is replay the detonation with the real projectile.
	 * 
	 * The latter case is handled by TornOff. We don't allow replicated projectiles to detonate if they're not fully
	 * spawned. Instead, we wait until they get torn off (which will always occur AFTER BeginPlay), then try to
	 * detonate them again. This second detonation should execute successfully, and will either result in a
	 * successful prediction (the owning client's authoritative projectile, which now has a linked fake projectile,
	 * detonated with sufficient accuracy to that fake projectile; i.e. the branch above) or a missed prediction
	 * that will be caught by one of the above branches.
	 */
	if (!LinkedFakeProjectile)
	{
		if (bHasSpawnedFully)
		{
			if (bLog)
			{
				UE_LOG(RsAbilityLog, Warning, TEXT("Missed prediction: Fake projectile detonated so early that it's already been destroyed and can't be found (corresponding auth projectile: (%s)). The fake projectile should have switched to the real one by now. Reconciling by using real projectile's detonation..."), *GetNameSafe(this));
			}

			return true;
		}
	}

	return false;
}

FVector ARsProjectile::GetAreaOfEffectOrigin() const
{
	// XY in local space; Z in world space.
	const FVector LocalOffset2D = AreaOffset * FVector(1.0f, 1.0f, 0.0);
	const FVector WorldOffset2D = CollisionComp->GetComponentRotation().RotateVector(LocalOffset2D);
	const FVector FinalOffset = WorldOffset2D + FVector(0.0f, 0.0f, AreaOffset.Z);
	return (CollisionComp->GetComponentLocation() + FinalOffset);
}

void ARsProjectile::ApplyEffectToTarget(const TSubclassOf<UGameplayEffect> GE, const AActor* Target, const FHitResult& Hit) const
{
	if (URsAbilitySystemComponent* TargetASC = URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(Target))
	{
		FGameplayEffectSpecHandle EffectSpec;

		if (URsAbilitySystemComponent* InstigatorASC = URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(Owner))
		{
			EffectSpec = InstigatorASC->MakeOutgoingSpecWithHitResult(GE, 1.0f, Hit);
		}
		/* Projectiles can be fired from actors without an ASC (e.g. environmental hazards). Use the target's ASC to
		 * apply effects in these situations. */
		else
		{
			EffectSpec = TargetASC->MakeOutgoingSpecWithHitResult(GE, 1.0f, Hit);
		}

		TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
	}
}

void ARsProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/* NOTE: ProjectileId, PlayerController, and SpawnTransform don't need to be marked dirty; they're set before
	 * replication and should never change. */

	constexpr bool bUsePushModel = true;

	FDoRepLifetimeParams OwnerOnlyParams{COND_OwnerOnly, REPNOTIFY_Always, bUsePushModel};
	DOREPLIFETIME_WITH_PARAMS_FAST(ARsProjectile, ProjectileId, OwnerOnlyParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ARsProjectile, PlayerController, OwnerOnlyParams);

	FDoRepLifetimeParams ReplicatedMovementParams{COND_SimulatedOrPhysics, REPNOTIFY_OnChanged, bUsePushModel};
	DOREPLIFETIME_WITH_PARAMS_FAST(ARsProjectile, ReplicatedProjectileMovement, ReplicatedMovementParams);

	FDoRepLifetimeParams SimulatedOnlyParams{COND_SimulatedOnly, REPNOTIFY_OnChanged, bUsePushModel};
	DOREPLIFETIME_WITH_PARAMS_FAST(ARsProjectile, SpawnTransform, SimulatedOnlyParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ARsProjectile, DetonationInfo, SimulatedOnlyParams);
}

void ARsProjectile::GetReplicatedCustomConditionState(FCustomPropertyConditionState& OutActiveState) const
{
	Super::GetReplicatedCustomConditionState(OutActiveState);

	DOREPCUSTOMCONDITION_ACTIVE_FAST(AActor, AttachmentReplication, bReplicateProjectileMovement);
}

void ARsProjectile::OnRep_DetonationInfo()
{
	if (!ensure(DetonationInfo.bDetonated))
	{
		return;
	}

	/* We don't do any kind of resimulation for the owning client because (A) It's important that they have an accurate
	 * version of the authoritative projectile (to check for missed predictions, get accurate hit detection and FX,
	 * etc.) and (B) It's okay if they never see their own projectile. We also don't do any resimulation for the server
	 * because it's also acting as the authoritative simulation of the projectile. */
	if (ProjectileId == INVALID_PROJECTILE_ID)
	{
		float RemainingDistance = (DetonationInfo.HitLocation - GetActorLocation()).Length();
		const float CurrentSpeed = (GetVelocity().IsZero() ? ProjectileMovement->InitialSpeed : GetVelocity().Length());
		float RemainingTime = RemainingDistance / CurrentSpeed;

		/* If this projectile detonated on spawn, rewind and resimulate it at a modified velocity to ensure it never
		 * detonates without ever being seen by other players. */
		if (!bHasSpawnedFully)
		{
			// Rewind the projectile now (instead of waiting for BeginPlay).
			SetActorTransform(SpawnTransform);

			// Modify this projectile's velocity to ensure it stays visible for at least MinLifeTime.
			RemainingDistance = (DetonationInfo.HitLocation - SpawnTransform.GetLocation()).Length();
			RemainingTime = FMath::Max((RemainingDistance / CurrentSpeed), MinLifetime);
			const float DesiredSpeed = (RemainingDistance / RemainingTime);
			const float SpeedQuotient = (DesiredSpeed / CurrentSpeed);

			// If we detonated on spawn, we might not have initialized our velocity yet.
			if (GetVelocity().IsZero())
			{
				ProjectileMovement->InitialSpeed *= SpeedQuotient;
			}
			else
			{
				ProjectileMovement->Velocity *= SpeedQuotient;
			}

			UE_LOG(RsAbilityLog, VeryVerbose, TEXT("Projectile (%s) detonated on spawn on non-owning simulated proxy. Rewinding and resimulating with a speed of (%f) for (%f) seconds."), *GetName(), DesiredSpeed, RemainingTime);
		}
		/* If this projectile detonated very close to its spawn, slow its velocity to ensure it stays visible long
		 * enough to be seen by players. */
		else if ((GetGameTimeSinceCreation() + RemainingTime) < MinLifetime)
		{
			RemainingTime = (MinLifetime - GetGameTimeSinceCreation());

			const float DesiredSpeed = (RemainingDistance / RemainingTime);
			const float SpeedQuotient = (DesiredSpeed / CurrentSpeed);

			ProjectileMovement->Velocity *= SpeedQuotient;

			UE_LOG(RsAbilityLog, VeryVerbose, TEXT("Projectile (%s) detonated before living for MinLifetime (%f) on non-owning simulated proxy. Finishing simulation at a speed of (%f) for (%f) seconds to make sure it lives long enough."), *GetName(), MinLifetime, DesiredSpeed, RemainingTime);
		}
		else
		{
			UE_LOG(RsAbilityLog, VeryVerbose, TEXT("Projectile (%s) on non-owning simulated proxy will stay alive for (%f) at its current speed of (%f) to finish its resimulation at a distance of (%f)."), *GetName(), RemainingTime, CurrentSpeed, RemainingDistance);
		}

		// Account for processing time this frame.
		RemainingTime -= GetWorld()->GetDeltaSeconds();

		auto DetonateDelayedFunc = [this]()
		{
			/* Make sure we detonate in the correct final location, since we skip OnRep_ReplicatedMovement on non-owning
			 * simulated proxies. */
			if (RootComponent && !RootComponent->GetAttachParent())
			{
				PostNetReceiveVelocity(GetReplicatedMovement().LinearVelocity);
				PostNetReceiveLocationAndRotation();
			}

			bFinishedResim = true;
			DetonateWithDetonationInfo();
		};

		/* If the time left in our resimulation is so short that it will finish before the next frame, skip the timer
		 * and detonate immediately. */
		if (RemainingTime > GetWorld()->GetDeltaSeconds())
		{
			GetWorldTimerManager().SetTimer(FinishedResimulationTimer, FTimerDelegate::CreateWeakLambda(this, DetonateDelayedFunc), RemainingTime, false);
		}
		else
		{
			GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, DetonateDelayedFunc));
		}
	}
}

void ARsProjectile::DetonateWithDetonationInfo()
{
	if (!ensure(DetonationInfo.bDetonated))
	{
		return;
	}

	/* We do some data validation because this is usually called shortly after detonation info has been replicated, so
	 * there may be rare situations where the pointers are no longer valid. */
	Detonate
	(
		DetonationInfo.bHasDirectImpactTarget && IsValid(DetonationInfo.OtherActor),
		IsValid(DetonationInfo.OtherActor) ? DetonationInfo.OtherActor : nullptr,
		IsValid(DetonationInfo.OtherComp) ? DetonationInfo.OtherComp : nullptr,
		DetonationInfo.HitLocation,
		DetonationInfo.HitNormal
	);
}

bool ARsProjectile::IsServerProjectile() const
{
	return (GetWorld()->GetNetMode() != NM_Client);
}

#if WITH_EDITOR
void ARsProjectile::DrawDebug()
{
	if (!IsPendingKillPending())
	{
		// if (const UGASDeveloperSettings* DevSettings = GetDefault<UGASDeveloperSettings>())
		// {
		// 	// Draw the server's authoritative projectile.
		// 	if (HasAuthority() && !bIsFakeProjectile && (DevSettings->ProjectileDebugMode > PredictedVersusClient))
		// 	{
		// 		DrawDebugSphere(GetWorld(), GetActorLocation(), CollisionComp->GetScaledSphereRadius() * 1.5f, 8, DevSettings->ServerProjectileColor, false, DevSettings->DrawTime);
		// 	}
		// 	// Draw the authoritative and/or fake projectile once they're linked.
		// 	else if (!HasAuthority() &&
		// 		(DevSettings->ProjectileDebugMode > None))
		// 	{
		// 		// If the projectiles are synced, only make one draw to make this clear.
		// 		if ((DevSettings->ProjectileDebugMode == PredictedVersusClient || DevSettings->ProjectileDebugMode == All) &&
		// 			IsValid(LinkedFakeProjectile) && (GetActorLocation() - LinkedFakeProjectile->GetActorLocation()).Size() < 0.05f)
		// 		{
		// 			DrawProjectileStep(DevSettings->SyncedColor, true);
		// 		}
		// 		else
		// 		{
		// 			// Draw the client's authoritative projectile.
		// 			if (DevSettings->ProjectileDebugMode > None)
		// 			{
		// 				DrawProjectileStep(DevSettings->ClientAuthoritativeProjectileColor, true);
		// 			}
		//
		// 			// Draw the client's fake projectile.
		// 			if (IsValid(LinkedFakeProjectile) &&
		// 				(DevSettings->ProjectileDebugMode == PredictedVersusClient || DevSettings->ProjectileDebugMode == All))
		// 			{
		// 				LinkedFakeProjectile->DrawProjectileStep(DevSettings->ClientFakeProjectileColor, true);
		//
		// 				/* Draw an arrow from the fake projectile to the authoritative projectile at each timestamp.
		// 				 * Randomize this color so we can distinguish each step. */
		// 				FColor ArrowColor = FColor::MakeRandomColor();
		// 				DrawDebugDirectionalArrow(GetWorld(), LinkedFakeProjectile->GetActorLocation(), GetActorLocation(), 30.0f, ArrowColor, false, 10.0f, 0, 0.5f);
		// 				DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), LinkedFakeProjectile->GetActorLocation(), 30.0f, ArrowColor, false, 10.0f, 0, 0.5f);
		// 			}
		// 		}
		// 	}
		// 	/* Draw the fake projectile if desired. We don't need to check bWaitForLinkage here (see BeginPlay), but
		// 	 * it's more efficient to do so. */
		// 	else if (!DevSettings->bWaitForLinkage)
		// 	{
		// 		if (HasAuthority() && GetInstigator() && GetInstigator()->IsLocallyControlled() &&
		// 			(DevSettings->ProjectileDebugMode == PredictedVersusClient || DevSettings->ProjectileDebugMode == All))
		// 		{
		// 			DrawProjectileStep(DevSettings->ClientFakeProjectileColor, true);
		// 		}
		// 	}
		// }
	}
}

void ARsProjectile::DrawDetonationInfo(const FVector& Location, const FVector& Normal) const
{
	// if (const UGASDeveloperSettings* DevSettings = GetDefault<UGASDeveloperSettings>())
	// {
	// 	if (DevSettings->bDrawFinalPosition)
	// 	{
	// 		const FColor Color = GetDebugColor();
	//
	// 		// Show the transform of the hit.
	// 		DrawDebugSphere(GetWorld(), Location, 1.0f, 8, Color, false, DevSettings->DrawTime, 0, 1.0f);
	// 		DrawDebugDirectionalArrow(GetWorld(), Location, (Location + (Normal * 25.0f)), 25.0f, Color, false, DevSettings->DrawTime, 0, 1.0f);
	//
	// 		// Show the AOE radius. AOE radius is only relevant on the server, where AOE effects are applied.
	// 		if (AreaRadius > 0.0f && IsServerProjectile())
	// 		{
	// 			DrawDebugSphere(GetWorld(), GetAreaOfEffectOrigin(), AreaRadius, (AreaRadius / 16.0f), Color, false, DevSettings->DrawTime);
	//
	// 			// If the sphere's origin isn't the projectile's location, draw it too.
	// 			if (AreaOffset.Size() > 0.0f)
	// 			{
	// 				DrawDebugSphere(GetWorld(), GetAreaOfEffectOrigin(), 2.0f, 8, Color, false, DevSettings->DrawTime, 0, 2.0f);
	// 			}
	// 		}
	// 	}
	// }
}

void ARsProjectile::DrawProjectileStep(const FColor& Color, bool bDrawDot, bool bThick) const
{
	DrawDebugCapsule(GetWorld(), GetActorLocation(), CollisionComp->GetScaledCapsuleHalfHeight(), CollisionComp->GetScaledCapsuleRadius(), GetActorQuat(), Color, bDrawDot, bThick);

	if (bDrawDot)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 1.0f, 8, Color, false, 2.f, 0, 1.0f);
	}
}

FColor ARsProjectile::GetDebugColor() const
{
	// const UGASDeveloperSettings* DevSettings = GetDefault<UGASDeveloperSettings>();
	// return (bIsFakeProjectile ? DevSettings->ClientFakeProjectileColor :
	// 		(IsServerProjectile() ? DevSettings->ServerProjectileColor :
	// 									DevSettings->ClientAuthoritativeProjectileColor));
	return FColor::Red;
}
#endif // WITH_EDITOR
