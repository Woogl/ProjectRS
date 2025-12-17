// Copyright 2024 Team BH.


#include "RsAnimNotify_EventToTarget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

URsAnimNotify_EventToTarget::URsAnimNotify_EventToTarget()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_EventToTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	PerformTargeting(MeshComp);

	if (!EventTag.IsValid())
	{
		return;
	}
	
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			for (AActor* Target : Targets)
			{
				FGameplayEventData Payload;
				Payload.EventTag = EventTag;
				Payload.Instigator = Owner;
				Payload.Target = Target;
				SourceASC->HandleGameplayEvent(EventTag, &Payload);
			}
		}
	}
}
