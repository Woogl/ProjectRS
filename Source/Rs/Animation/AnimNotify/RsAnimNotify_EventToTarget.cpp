// Copyright 2024 Team BH.


#include "RsAnimNotify_EventToTarget.h"

#include "AbilitySystemComponent.h"
#include "Rs/Condition/RsCondition.h"

URsAnimNotify_EventToTarget::URsAnimNotify_EventToTarget()
{
}

void URsAnimNotify_EventToTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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

	TArray<AActor*> Targets;
	if (URsCondition_Targeting* TargetingCondition = Cast<URsCondition_Targeting>(Condition))
	{
		// Share targeting result
		Targets = TargetingCondition->OutActors;
	}
	
	for (AActor* Target : Targets)
	{
		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.Instigator = OwnerASC->GetOwnerActor();
		Payload.Target = Target;
		OwnerASC->HandleGameplayEvent(EventTag, &Payload);
	}
}
