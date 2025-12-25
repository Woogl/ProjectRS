// Copyright 2024 Team BH.


#include "RsAnimNotify_EventToSelf.h"

#include "AbilitySystemComponent.h"
#include "Rs/Condition/RsCondition.h"

URsAnimNotify_EventToSelf::URsAnimNotify_EventToSelf()
{
}

void URsAnimNotify_EventToSelf::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (Condition && !Condition->IsSatisfied(MeshComp->GetOwner()))
	{
		return;
	}

	if (!EventTag.IsValid() || !OwnerASC.IsValid())
	{
		return;
	}
	
	FGameplayEventData Payload;
	Payload.EventTag = EventTag;
	Payload.Instigator = OwnerASC->GetOwnerActor();
	Payload.Target = OwnerASC->GetOwnerActor();
	OwnerASC->HandleGameplayEvent(EventTag, &Payload);
}
