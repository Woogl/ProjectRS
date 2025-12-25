// Copyright 2025 Team BH.


#include "RsAnimNotifyBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/Condition/RsCondition.h"
#include "Rs/Condition/RsConditionTask.h"

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
	
	bHasTriggered = false;
	OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (const UAbilitySystemComponent* ASC = OwnerASC.Get())
	{
		CurrentAbility = ASC->GetAnimatingAbility();
	}

	if (URsConditionTaskBase* ConditionTrigger = Cast<URsConditionTaskBase>(Condition))
	{
		ConditionTrigger->Activate(Owner);
		ConditionTrigger->OnTriggered.AddDynamic(this, &ThisClass::HandleConditionTriggered);
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
	
	if (URsConditionBase* CastedCondition = Cast<URsConditionBase>(Condition))
	{
		if (CastedCondition->IsSatisfied(Owner))
		{
			HandleConditionTriggered();
		}
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
	
	if (URsConditionTaskBase* ConditionTrigger = Cast<URsConditionTaskBase>(Condition))
	{
		ConditionTrigger->Deactivate(Owner);
		ConditionTrigger->OnTriggered.RemoveAll(this);
	}
}

void URsAnimNotifyStateBase::HandleConditionTriggered()
{
	bHasTriggered = true;
	
	// Do something in child class.
}
