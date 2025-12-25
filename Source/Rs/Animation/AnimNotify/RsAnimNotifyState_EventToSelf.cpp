// Copyright 2025 Team BH.


#include "RsAnimNotifyState_EventToSelf.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"

URsAnimNotifyState_EventToSelf::URsAnimNotifyState_EventToSelf()
{
}

void URsAnimNotifyState_EventToSelf::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (CurrentAbility.IsValid())
	{
		if (URsGameplayAbility* RsAbility = Cast<URsGameplayAbility>(CurrentAbility))
		{
			RsAbility->RevertGameplayEvent(EventTag);
		}
	}
}

void URsAnimNotifyState_EventToSelf::HandleConditionSatisfied()
{
	if (UAbilitySystemComponent* ASC = OwnerASC.Get())
	{
		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.EventMagnitude = CachedTotalDuration;
		Payload.Instigator = ASC->GetOwnerActor();
		Payload.Target = ASC->GetOwnerActor();
		ASC->HandleGameplayEvent(EventTag, &Payload);
	}
	
	Super::HandleConditionSatisfied();
}
