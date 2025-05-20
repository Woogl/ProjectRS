// Copyright 2024 Team BH.


#include "RsAnimNotifyState_EffectToSelf.h"

#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectDefinition.h"

void URsAnimNotifyState_EffectToSelf::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			EffectDefinition->ApplyEffect(ASC, ASC);
		}
	}
}
