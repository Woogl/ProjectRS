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
	PrimaryComponentTick.bCanEverTick = false;
}

bool URsLockOnComponent::ToggleLockOn()
{
	if (GetLockOnTarget() == nullptr)
	{
		TArray<AActor*> OutActors;
		if (URsTargetingLibrary::PerformTargeting(GetOwner(), GetOwner()->GetActorTransform(), TargetingCollision, TargetingFilter, TargetingSorter, OutActors))
		{
			LockOn(OutActors[0]);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		LockOff();
		return false;
	}
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

	LockedOnTarget = TargetActor;
	if (LockedOnTarget.IsValid())
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

		if (URsHealthComponent* HealthComponent = LockedOnTarget.Get()->FindComponentByClass<URsHealthComponent>())
		{
			HealthComponent->OnDeathStarted.AddUniqueDynamic(this, &ThisClass::HandleDeathStarted);
		}
	}
	
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(GetOwner()))
	{
		RsPlayerController->CameraRig = ERsCameraRig::LockOn;
	}
}

void URsLockOnComponent::LockOff()
{
	if (LockedOnTarget.IsValid())
	{
		if (URsHealthComponent* HealthComponent = LockedOnTarget.Get()->FindComponentByClass<URsHealthComponent>())
		{
			HealthComponent->OnDeathStarted.RemoveAll(this);
		}
		LockedOnTarget.Reset();
	}

	if (ReticleComponent.IsValid())
	{
		ReticleComponent.Get()->DestroyComponent();
	}
	
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(GetOwner()))
	{
		RsPlayerController->CameraRig = ERsCameraRig::ThirdPerson;
	}
}

AActor* URsLockOnComponent::GetLockOnTarget() const
{
	return LockedOnTarget.Get();
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

