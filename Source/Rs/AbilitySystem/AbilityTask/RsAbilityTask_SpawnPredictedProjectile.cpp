// Copyright 2025 Team BH.


#include "RsAbilityTask_SpawnPredictedProjectile.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/Battle/Actor/RsProjectile.h"
#include "Rs/Player/RsProjectilePredictionComponent.h"

UScriptStruct* FGameplayAbilityTargetData_ProjectileSpawnInfo::GetScriptStruct() const
{
	return FGameplayAbilityTargetData_ProjectileSpawnInfo::StaticStruct();
}

FString FGameplayAbilityTargetData_ProjectileSpawnInfo::ToString() const
{
	return FString::Printf(TEXT("FGameplayAbilityTargetData_ProjectileSpawnInfo: (%i)"), ProjectileId);
}

FGameplayAbilityTargetDataHandle FGameplayAbilityTargetData_ProjectileSpawnInfo::MakeProjectileSpawnInfoTargetData(const FVector& SpawnLocation, const FRotator& SpawnRotation, const uint32 ProjectileId)
{
	FGameplayAbilityTargetData_ProjectileSpawnInfo* TargetData = new FGameplayAbilityTargetData_ProjectileSpawnInfo();
	TargetData->SpawnLocation = SpawnLocation;
	TargetData->SpawnRotation = SpawnRotation;
	TargetData->ProjectileId = ProjectileId;
	FGameplayAbilityTargetDataHandle Handle;
	Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData_ProjectileSpawnInfo>(TargetData));
	return Handle;
}

bool FGameplayAbilityTargetData_ProjectileSpawnInfo::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	Ar << SpawnLocation;
	Ar << SpawnRotation;
	Ar << ProjectileId;
	bOutSuccess = true;
	return true;
}

URsAbilityTask_SpawnPredictedProjectile* URsAbilityTask_SpawnPredictedProjectile::SpawnPredictedProjectile(UGameplayAbility* OwningAbility, TSubclassOf<ARsProjectile> ProjectileClass, FVector SpawnLocation, FRotator SpawnRotation)
{
	if (!ensureAlwaysMsgf(OwningAbility->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted, TEXT("SpawnPredictedProjectile ability task activated in ability (%s), which does not have a net execution policy of Local Predicted. SpawnPredictedProjectile should only be used in predicted abilities. Use AbilityTask_SpawnActor otherwise."), *GetNameSafe(OwningAbility)))
	{
		return nullptr;
	}
	if (!ensureAlwaysMsgf(IsValid(ProjectileClass), TEXT("SpawnPredictedProjectile ability task activated in ability (%s) without a valid projectile class set."), *GetNameSafe(OwningAbility)))
	{
		return nullptr;
	}
	URsAbilityTask_SpawnPredictedProjectile* Task = NewAbilityTask<URsAbilityTask_SpawnPredictedProjectile>(OwningAbility);
	Task->ProjectileClass = ProjectileClass;
	Task->SpawnLocation = SpawnLocation;
	Task->SpawnRotation = SpawnRotation;
	return Task;
}

void URsAbilityTask_SpawnPredictedProjectile::Activate()
{
	Super::Activate();
	
	// On the client, listen for if this task is rejected. If it is, we need to destroy our fake projectile.
	if (IsPredictingClient())
	{
		GetActivationPredictionKey().NewRejectedDelegate().BindUObject(this, &ThisClass::OnTaskRejected);
	}
	
	if (Ability && Ability->GetCurrentActorInfo())
	{
		if (APlayerController* PlayerController = GetPlayerController())
		{
			const float ForwardPredictionTime = GetProjectilePredictionComponent()->GetForwardPredictionTime();
			const bool bIsNetAuthority = Ability->GetCurrentActorInfo()->IsNetAuthority();
			// The server can spawn the actor without waiting for the client's data if it's a listen server or a standalone client.
			const bool bShouldUseServerInfo = IsLocallyControlled();
			// On the server (if it's not a listen server), wait for the client to send us the projectile's spawn info.
			if (bIsNetAuthority && !bShouldUseServerInfo)
			{
				const FGameplayAbilitySpecHandle& SpecHandle = GetAbilitySpecHandle();
				const FPredictionKey& ActivationPredictionKey = GetActivationPredictionKey();
				AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &ThisClass::OnSpawnDataReplicated);
				AbilitySystemComponent->AbilityTargetDataCancelledDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &ThisClass::OnSpawnDataCancelled);
				// Check if the client already sent the data.
				AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
				// Kill the ability if we never receive the data.
				SetWaitingOnRemotePlayerData();
				return;
			}
			
			/* On the client, if our ping is too high to forward-predict, delay spawning the projectile so we don't
			 * forward-predict further than MaxPredictionPing. */
			if (!bIsNetAuthority)
			{
				// We wait just enough to spawn the projectile with the maximum possible forward-prediction. 
				float SleepTime = GetProjectilePredictionComponent()->GetProjectileSleepTime();
				if (SleepTime > 0.0f)
				{
					if (!GetWorld()->GetTimerManager().IsTimerActive(SpawnDelayedFakeProjHandle))
					{
						// Set a timer to spawn the predicted projectile after a delay.
						DelayedProjectileInfo.ProjectileClass = ProjectileClass;
						DelayedProjectileInfo.SpawnLocation = SpawnLocation;
						DelayedProjectileInfo.SpawnRotation = SpawnRotation;
						DelayedProjectileInfo.PlayerController = PlayerController;
						DelayedProjectileInfo.ProjectileId = GetProjectilePredictionComponent()->GenerateNewFakeProjectileId();
						GetWorld()->GetTimerManager().SetTimer(SpawnDelayedFakeProjHandle, this, &ThisClass::SpawnDelayedFakeProjectile, SleepTime, false);
						UE_LOG(RsAbilityLog, Verbose, TEXT("(%i:%i.%i) (ID: %i): Spawning fake projectile delayed. Ping (%fms) exceeds maximum prediction time. Sleeping for (%fms) to forward-predict with maximum time (%fms) and latency reduction (%fms)."), FDateTime::UtcNow().GetMinute(), FDateTime::UtcNow().GetSecond(), FDateTime::UtcNow().GetMillisecond(), DelayedProjectileInfo.ProjectileId, PlayerController->PlayerState->ExactPing, SleepTime * 1000.f, ForwardPredictionTime * 1000.f, GetProjectilePredictionComponent()->PredictionLatencyReduction);
					}
					return;
				}
			}
			
			/* On the client, if our ping is low enough to forward-predict (or we're on LAN), immediately spawn and 
			 * initialize the fake projectile. */
			if (!bIsNetAuthority)
			{
				const uint32 FakeProjectileId = GetProjectilePredictionComponent()->GenerateNewFakeProjectileId();
				if (ARsProjectile* NewProjectile = GetWorld()->SpawnActor<ARsProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, GenerateSpawnParamsForFake(FakeProjectileId)))
				{
					UE_LOG(RsAbilityLog, Verbose, TEXT("(%i:%i.%i) (ID: %i): Successfully spawned fake projectile (%s) on time. Attempting to forward-predict (%fms) with ping (%fms). Client bias: (%i%%)."), FDateTime::UtcNow().GetMinute(), FDateTime::UtcNow().GetSecond(), FDateTime::UtcNow().GetMillisecond(), FakeProjectileId, *GetNameSafe(NewProjectile), ForwardPredictionTime * 1000.0f, PlayerController->PlayerState->ExactPing, (uint32)(GetProjectilePredictionComponent()->ClientBiasPct * 100.f));
					// Send the spawn info to the server so it can spawn the authoritative projectile.
					SendSpawnDataToServer(SpawnLocation, SpawnRotation, FakeProjectileId);
					// Cache the projectile in case the server rejects this task, and we have to destroy it.
					SpawnedFakeProj = NewProjectile;
					if (ShouldBroadcastAbilityTaskDelegates())
					{
						Success.Broadcast(NewProjectile);
					}
					// We don't end the task here because we need to keep listening for possible rejection.
					return;
				}
			}
			
			/* On listen servers or in standalone, spawn the authoritative projectile. No prediction is needed in this
			 * case. Remote servers don't spawn authoritative actors until OnTargetDataReplicated. */
			else if (bIsNetAuthority && bShouldUseServerInfo)
			{
				if (ARsProjectile* NewProjectile = GetWorld()->SpawnActor<ARsProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, GenerateSpawnParams()))
				{
					UE_LOG(RsAbilityLog, Verbose, TEXT("(%i:%i.%i) (ID: N/A): Successfully spawned authoritative projectile (%s) on time for local server. No prediction performed."), FDateTime::UtcNow().GetMinute(), FDateTime::UtcNow().GetSecond(), FDateTime::UtcNow().GetMillisecond(), *GetNameSafe(NewProjectile));
					if (ShouldBroadcastAbilityTaskDelegates())
					{
						Success.Broadcast(NewProjectile);
					}
					EndTask();
					return;
				}
			}
		}
	}
	
	// Cancel the task on the server if the client failed.
	CancelServerSpawn();
	
	// Failed to spawn. Ability should usually be canceled at this point.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FailedToSpawn.Broadcast(nullptr);
	}
	
	EndTask();
}

void URsAbilityTask_SpawnPredictedProjectile::SpawnDelayedFakeProjectile()
{
	if (Ability && Ability->GetCurrentActorInfo() && DelayedProjectileInfo.PlayerController.IsValid())
	{
		if (ARsProjectile* NewProjectile = GetWorld()->SpawnActor<ARsProjectile>(DelayedProjectileInfo.ProjectileClass, DelayedProjectileInfo.SpawnLocation, DelayedProjectileInfo.SpawnRotation, GenerateSpawnParamsForFake(DelayedProjectileInfo.ProjectileId)))
		{
			UE_LOG(RsAbilityLog, Verbose, TEXT("(%i:%i.%i) (ID: %i): Successfully spawned fake projectile (%s) delayed. Attempting to forward-predict (%fms) with ping (%fms)."), FDateTime::UtcNow().GetMinute(), FDateTime::UtcNow().GetSecond(), FDateTime::UtcNow().GetMillisecond(), DelayedProjectileInfo.ProjectileId, *GetNameSafe(NewProjectile), DelayedProjectileInfo.PlayerController->FindComponentByClass<URsProjectilePredictionComponent>()->GetForwardPredictionTime() * 1000.0f, DelayedProjectileInfo.PlayerController->PlayerState->ExactPing);
			// Send the spawn info to the server so it can spawn the authoritative projectile.
			SendSpawnDataToServer(DelayedProjectileInfo.SpawnLocation, DelayedProjectileInfo.SpawnRotation, DelayedProjectileInfo.ProjectileId);
			SpawnedFakeProj = NewProjectile;
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				Success.Broadcast(NewProjectile);
			}
			// We don't end the task here because we need to keep listening for possible rejection.
			return;
		}
	}
	
	// Cancel the task on the server if the client failed.
	CancelServerSpawn();
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FailedToSpawn.Broadcast(nullptr);
	}
	
	EndTask();
}

void URsAbilityTask_SpawnPredictedProjectile::SendSpawnDataToServer(const FVector& InLocation, const FRotator& InRotation, uint32 InProjectileId)
{
	const bool bGenerateNewKey = !AbilitySystemComponent->ScopedPredictionKey.IsValidForMorePrediction();
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get(), bGenerateNewKey);
	FGameplayAbilityTargetDataHandle Handle = FGameplayAbilityTargetData_ProjectileSpawnInfo::MakeProjectileSpawnInfoTargetData(InLocation, InRotation, InProjectileId);
	AbilitySystemComponent->CallServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), Handle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

}

void URsAbilityTask_SpawnPredictedProjectile::CancelServerSpawn()
{
	const bool bGenerateNewKey = !AbilitySystemComponent->ScopedPredictionKey.IsValidForMorePrediction();
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get(), bGenerateNewKey);
	AbilitySystemComponent->ServerSetReplicatedTargetDataCancelled(GetAbilitySpecHandle(), GetActivationPredictionKey(), AbilitySystemComponent->ScopedPredictionKey);
}

void URsAbilityTask_SpawnPredictedProjectile::OnSpawnDataReplicated(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag Activation)
{
	// Copy the target data before we consume it.
	const FGameplayAbilityTargetData* TargetData = Data.Get(0);
	// Consume the client's data. Ensures each server task only spawns one projectile for each client task.
	if (!Cast<URsAbilitySystemComponent>(AbilitySystemComponent)->TryConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey()))
	{
		return;
	}
	if (TargetData)
	{
		if (const FGameplayAbilityTargetData_ProjectileSpawnInfo* SpawnInfo = static_cast<const FGameplayAbilityTargetData_ProjectileSpawnInfo*>(TargetData))
		{
			const float ForwardPredictionTime = GetProjectilePredictionComponent()->GetForwardPredictionTime();
			if (ARsProjectile* NewProjectile = GetWorld()->SpawnActor<ARsProjectile>(ProjectileClass, SpawnInfo->SpawnLocation, SpawnInfo->SpawnRotation, GenerateSpawnParamsForAuth(SpawnInfo->ProjectileId)))
			{
				// Note that there will be a discrepancy between the server's perceived ping and the client's.
				//UE_LOG(RsAbilityLog, Verbose, TEXT("(%i:%i.%i) (ID: %i): Successfully spawned authoritative projectile (%s). Forwarded (%fms) for perceived ping (%fms). Latency reduction: (%fms) Client bias: (%i%%)"), FDateTime::UtcNow().GetMinute(), FDateTime::UtcNow().GetSecond(), FDateTime::UtcNow().GetMillisecond(), SpawnInfo->ProjectileId, *GetNameSafe(NewProjectile), ForwardPredictionTime * 1000.0f, CrashPC->PlayerState->ExactPing, CrashPC->PredictionLatencyReduction, (uint32)(CrashPC->ClientBiasPct * 100.0f));
				if (NewProjectile->ProjectileMovement)
				{
					// Tick the actor (e.g. animations, VFX).
					if (NewProjectile->PrimaryActorTick.IsTickFunctionEnabled())
					{
						NewProjectile->TickActor(ForwardPredictionTime * NewProjectile->CustomTimeDilation, LEVELTICK_All, NewProjectile->PrimaryActorTick);
					}
					// Tick the movement component (to actually move the projectile).
					NewProjectile->ProjectileMovement->TickComponent(ForwardPredictionTime * NewProjectile->CustomTimeDilation, LEVELTICK_All, nullptr);
					if (NewProjectile->GetLifeSpan() > 0.0f)
					{
						/* Since we're fast-forwarding this actor, we need to subtract the forward prediction time from
						 * its lifespan. Clamp at 0.2 so we have enough time to replicate. */
						NewProjectile->SetLifeSpan(FMath::Max(0.2f, NewProjectile->GetLifeSpan() - ForwardPredictionTime));
					}
				}
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					Success.Broadcast(NewProjectile);
				}
				EndTask();
				return;
			}
		}
	}
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FailedToSpawn.Broadcast(nullptr);
	}
	
	EndTask();
}

void URsAbilityTask_SpawnPredictedProjectile::OnSpawnDataCancelled()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FailedToSpawn.Broadcast(nullptr);
	}
	EndTask();
}

void URsAbilityTask_SpawnPredictedProjectile::OnTaskRejected()
{
	UE_LOG(RsAbilityLog, Warning, TEXT("(ID: %s): SpawnPredictedProjectile task in ability (%s) rejected. Destroying fake projectile (%s)..."), *GetNameSafe(SpawnedFakeProj.Get()), *GetNameSafe(Ability), *GetNameSafe(SpawnedFakeProj.Get()));
	// If we've spawned a fake projectile on the client, destroy it.
	if (SpawnedFakeProj.IsValid())
	{
		// The fake projectile will still be lingering on the PC's list of unlinked projectiles; we need to remove it.
		if (URsProjectilePredictionComponent* PredComp = GetProjectilePredictionComponent())
		{
			if (const uint32* Key = PredComp->FakeProjectiles.FindKey(SpawnedFakeProj.Get()))
			{
				UE_LOG(RsAbilityLog, Error, TEXT("Removed %i"), *Key);
				PredComp->FakeProjectiles.Remove(*Key);
			}
		}
		SpawnedFakeProj.Get()->Destroy();
	}
	// If we didn't spawn the fake projectile yet (because we're waiting for a delayed spawn), cancel it.
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

FActorSpawnParameters URsAbilityTask_SpawnPredictedProjectile::GenerateSpawnParams() const
{
	FActorSpawnParameters Params;
	Params.Instigator = Ability->GetCurrentActorInfo()->AvatarActor.IsValid() ? Cast<APawn>(Ability->GetCurrentActorInfo()->AvatarActor) : nullptr;
	Params.Owner = AbilitySystemComponent->GetOwnerActor();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return Params;
}

FActorSpawnParameters URsAbilityTask_SpawnPredictedProjectile::GenerateSpawnParamsForFake(const uint32 ProjectileId) const
{
	// Fake projectiles need to be marked as fake before BeginPlay.
	FActorSpawnParameters Params = GenerateSpawnParams();
	APlayerController* PlayerController = GetPlayerController();
	Params.CustomPreSpawnInitalization = [ProjectileId, PlayerController](AActor* Actor)
	{
		if (ARsProjectile* Projectile = Cast<ARsProjectile>(Actor))
		{
			Projectile->InitProjectileId(ProjectileId);
			Projectile->InitFakeProjectile(PlayerController, ProjectileId);
		}
	};
	return Params;
}

FActorSpawnParameters URsAbilityTask_SpawnPredictedProjectile::GenerateSpawnParamsForAuth(const uint32 ProjectileId) const
{
	FActorSpawnParameters Params = GenerateSpawnParams();
	Params.CustomPreSpawnInitalization = [ProjectileId](AActor* Actor)
	{
		if (ARsProjectile* Projectile = Cast<ARsProjectile>(Actor))
		{
			Projectile->InitProjectileId(ProjectileId);
		}
	};
	return Params;
}

APlayerController* URsAbilityTask_SpawnPredictedProjectile::GetPlayerController() const
{
	if (Ability && Ability->GetCurrentActorInfo())
	{
		return Ability->GetCurrentActorInfo()->PlayerController.Get();
	}
	return nullptr;
}

URsProjectilePredictionComponent* URsAbilityTask_SpawnPredictedProjectile::GetProjectilePredictionComponent() const
{
	if (const APlayerController* PlayerController = GetPlayerController())
	{
		return PlayerController->FindComponentByClass<URsProjectilePredictionComponent>();
	}
	return nullptr;
}
