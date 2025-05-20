// Copyright 2024 Team BH.


#include "RsAnimNotifyState_EffectToTarget.h"

#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectDefinition.h"

void URsAnimNotifyState_EffectToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			for (AActor* ResultActor : Targets)
			{
				if (UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ResultActor))
				{
					EffectDefinition->ApplyEffect(SourceASC, TargetASC);
				}
			}
		}
	}
}
