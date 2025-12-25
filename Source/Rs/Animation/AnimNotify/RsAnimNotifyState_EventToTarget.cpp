// Copyright 2025 Team BH.


#include "RsAnimNotifyState_EventToTarget.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/Condition/RsCondition.h"

URsAnimNotifyState_EventToTarget::URsAnimNotifyState_EventToTarget()
{
}

void URsAnimNotifyState_EventToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UGameplayAbility* Ability = CurrentAbility.Get())
	{
		if (URsGameplayAbility* RsAbility = Cast<URsGameplayAbility>(Ability))
		{
			RsAbility->RevertGameplayEvent(EventTag);
		}
	}
}

void URsAnimNotifyState_EventToTarget::HandleConditionSatisfied()
{
	TArray<AActor*> Targets;
	if (URsCondition_Targeting* TargetingCondition = Cast<URsCondition_Targeting>(Condition))
	{
		// Share targeting result
		Targets = TargetingCondition->OutActors;
	}

	if (OwnerASC.IsValid())
	{
		for (AActor* Target : Targets)
		{
			FGameplayEventData Payload;
			Payload.EventTag = EventTag;
			Payload.EventMagnitude = CachedTotalDuration;
			Payload.Instigator = OwnerASC->GetOwnerActor();
			Payload.Target = Target;
			OwnerASC->HandleGameplayEvent(EventTag, &Payload);
		}
	}
	
	Super::HandleConditionSatisfied();
}
