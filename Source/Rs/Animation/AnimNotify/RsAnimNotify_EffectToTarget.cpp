// Copyright 2024 Team BH.


#include "RsAnimNotify_EffectToTarget.h"

#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectDefinition.h"
#include "Rs/Battle/RsBattleLibrary.h"

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
				if (UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target))
				{
					EffectDefinition->ApplyEffect(SourceASC, TargetASC);
				}
			}
		}
	}
}
