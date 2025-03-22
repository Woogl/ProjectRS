// Copyright 2024 Team BH.


#include "RsLockOnComponent.h"

#include "AbilitySystemGlobals.h"
#include "RsLockOnInterface.h"
#include "Components/WidgetComponent.h"
#include "Rs/AbilitySystem/Component/RsHealthComponent.h"

URsLockOnComponent::URsLockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
	
	LockedOnTarget = TargetActor;

	if (ReticleComponent.IsValid())
	{
		// Destroy old reticle widdget.
		ReticleComponent.Get()->DestroyComponent();
	}

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
			HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::HandleDeathStarted);
		}
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

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		FGameplayTagContainer LockOnAbilityTags = LockOnAbilityTag.GetSingleTagContainer();
		ASC->CancelAbilities(&LockOnAbilityTags, nullptr);
	}
}

bool URsLockOnComponent::HasLockOnTarget() const
{
	return LockedOnTarget.IsValid();
}

AActor* URsLockOnComponent::GetLockedOnTarget() const
{
	return LockedOnTarget.Get();
}

void URsLockOnComponent::HandleDeathStarted(AActor* DeadActor)
{
	LockOff();
	
	// ReActivate Lock on ability.
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
		ASC->TryActivateAbilitiesByTag(LockOnAbilityTag.GetSingleTagContainer());
	});
}

