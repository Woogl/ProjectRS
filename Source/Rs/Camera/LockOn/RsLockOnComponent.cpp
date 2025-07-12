// Copyright 2024 Team BH.


#include "RsLockOnComponent.h"

#include "AbilitySystemGlobals.h"
#include "RsLockOnInterface.h"
#include "Components/WidgetComponent.h"
#include "Rs/Character/Component/RsHealthComponent.h"
#include "Rs/Player/RsPlayerController.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsLockOnComponent::URsLockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void URsLockOnComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Target = GetLockOnTarget();
	if (!Target)
	{
		return;
	}
	AController* Controller = Cast<AController>(GetOwner());
	if (!Controller)
	{
		return;
	}
	APawn* ControlledPawn = Controller->GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	FVector TargetLocation = Target->GetActorLocation();
	FVector PawnLocation = ControlledPawn->GetActorLocation();
	double Distance = FVector::Dist(TargetLocation, PawnLocation);
	if (Distance > MaxTargetDistance)
	{
		LockOff();
	}
}

bool URsLockOnComponent::ToggleLockOn()
{
	if (GetLockOnTarget() == nullptr)
	{
		if (AController* Controller = Cast<AController>(GetOwner()))
		{
			if (APawn* ControlledPawn = Controller->GetPawn())
			{
				TArray<AActor*> OutActors;
				if (URsTargetingLibrary::PerformTargeting(ControlledPawn, ControlledPawn->GetTransform(), TargetingCollision, TargetingFilter, TargetingSorter, OutActors))
				{
					LockOn(OutActors[0]);
					return true;
				}
			}
		}
	}
	LockOff();
	return false;
}

void URsLockOnComponent::LockOn(AActor* TargetActor)
{
	if (IRsLockOnInterface* LockOnInterface = Cast<IRsLockOnInterface>(TargetActor))
	{
		if (LockOnInterface->Execute_IsLockableTarget(TargetActor) == false)
		{
			return;
		}
	}

	if (ReticleComponent.IsValid())
	{
		// Destroy old reticle widdget.
		ReticleComponent.Get()->DestroyComponent();
	}

	LockOnTarget = TargetActor;
	if (LockOnTarget.IsValid())
	{
		// Create new reticle widget.
		ReticleComponent = NewObject<UWidgetComponent>(TargetActor);
		if (ReticleComponent.IsValid())
		{
			if (ReticleWidget)
			{
				ReticleComponent->SetWidgetClass(ReticleWidget);
			}
			ReticleComponent->SetWidgetSpace(EWidgetSpace::Screen);
			ReticleComponent->SetDrawSize(ReticleDrawSize);
			UMeshComponent* MeshComponent = TargetActor->FindComponentByClass<UMeshComponent>();
			USceneComponent* ParentComponent = (MeshComponent && ReticleWidgetSocket != NAME_None) ? MeshComponent : TargetActor->GetRootComponent();
			FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, true);
			ReticleComponent->AttachToComponent(ParentComponent, AttachmentRule, ReticleWidgetSocket);
			ReticleComponent->SetVisibility(true);
			ReticleComponent->RegisterComponent();
		}

		if (URsHealthComponent* HealthComponent = LockOnTarget.Get()->FindComponentByClass<URsHealthComponent>())
		{
			HealthComponent->OnDeathStarted.AddUniqueDynamic(this, &ThisClass::HandleDeathStarted);
		}
	}
	
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(GetOwner()))
	{
		RsPlayerController->CameraRig = ERsCameraRig::LockOn;
	}

	SetComponentTickEnabled(true);
}

void URsLockOnComponent::LockOff()
{
	if (LockOnTarget.IsValid())
	{
		if (URsHealthComponent* HealthComponent = LockOnTarget.Get()->FindComponentByClass<URsHealthComponent>())
		{
			HealthComponent->OnDeathStarted.RemoveAll(this);
		}
		LockOnTarget.Reset();
	}

	if (ReticleComponent.IsValid())
	{
		ReticleComponent.Get()->DestroyComponent();
	}
	
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(GetOwner()))
	{
		RsPlayerController->CameraRig = ERsCameraRig::ThirdPerson;
	}
	
	SetComponentTickEnabled(false);
}

AActor* URsLockOnComponent::GetLockOnTarget() const
{
	return LockOnTarget.Get();
}

void URsLockOnComponent::HandleDeathStarted(AActor* DeadActor)
{
	LockOff();
	
	// Enemy will dead in next tick, so activate GA_LockOn next tick.
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
		{
			ASC->TryActivateAbilitiesByTag(LockOnAbilityTag.GetSingleTagContainer());
		}
	});
}

