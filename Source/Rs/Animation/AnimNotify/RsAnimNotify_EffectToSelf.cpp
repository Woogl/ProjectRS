// Copyright 2024 Team BH.


#include "RsAnimNotify_EffectToSelf.h"

#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectDefinition.h"
#include "Rs/Battle/RsBattleLibrary.h"

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
			EffectDefinition->ApplyEffect(ASC, ASC);
		}
	}
}
