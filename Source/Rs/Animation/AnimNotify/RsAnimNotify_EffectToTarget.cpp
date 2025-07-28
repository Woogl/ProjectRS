// Copyright 2024 Team BH.


#include "RsAnimNotify_EffectToTarget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

URsAnimNotify_EffectToTarget::URsAnimNotify_EffectToTarget()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_EffectToTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

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
