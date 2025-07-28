// Copyright 2024 Team BH.


#include "RsAnimNotify_EffectToSelf.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

URsAnimNotify_EffectToSelf::URsAnimNotify_EffectToSelf()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_EffectToSelf::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			FGameplayEventData Payload;
			Payload.EventTag = EventTag;
			Payload.Instigator = Owner;
			Payload.Target = Owner;
			ASC->HandleGameplayEvent(EventTag, &Payload);
		}
	}
}
