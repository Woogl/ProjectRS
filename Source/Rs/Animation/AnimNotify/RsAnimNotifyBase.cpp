// Copyright 2025 Team BH.


#include "RsAnimNotifyBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/Condition/RsCondition.h"
#include "Rs/Condition/RsTriggerCondition.h"

void URsAnimNotifyBase::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (const AActor* Owner = MeshComp->GetOwner())
	{
		OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
		if (const UAbilitySystemComponent* ASC = OwnerASC.Get())
		{
			CurrentAbility = ASC->GetAnimatingAbility();
		}
	}
}

void URsAnimNotifyStateBase::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
#if WITH_EDITOR
	SocketNames = MeshComp->GetAllSocketNames();
#endif // WITH_EDITOR
	
	bHasTriggered = false;
	OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	CachedTotalDuration = TotalDuration;
	if (const UAbilitySystemComponent* ASC = OwnerASC.Get())
	{
		CurrentAbility = ASC->GetAnimatingAbility();
	}
	
	if (!Condition && !Trigger)
	{
		// No condition and trigger means always satisfied. 
		HandleConditionSatisfied();
		return;
	}

	if (Condition && Condition->IsSatisfied(Owner))
	{
		HandleConditionSatisfied();
	}
	
	if (bShouldOnlyTriggerOnce && bHasTriggered)
	{
		return;
	}

	if (Trigger)
	{
		// Register condition to satisfied.
		Trigger->Initialize(Owner);
		Trigger->OnTriggered.AddUniqueDynamic(this, &ThisClass::HandleConditionSatisfied);
	}
}

void URsAnimNotifyStateBase::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	if (bShouldOnlyTriggerOnce && bHasTriggered)
	{
		return;
	}
	
	if (Condition && Condition->IsSatisfied(Owner))
	{
		HandleConditionSatisfied();
	}
}

void URsAnimNotifyStateBase::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
	if (Trigger)
	{
		Trigger->Deinitialize(Owner);
		Trigger->OnTriggered.RemoveAll(this);
	}
}

void URsAnimNotifyStateBase::HandleConditionSatisfied()
{
	// Do something in child class.
	
	bHasTriggered = true;
	if (bShouldOnlyTriggerOnce && Trigger)
	{
		Trigger->OnTriggered.RemoveAll(this);
	}
}
