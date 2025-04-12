// Copyright 2024 Team BH.


#include "RsAnimNotify_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility_Attack.h"

void URsAnimNotify_ActivateAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			if (UGameplayAbility* CurrentAbility = OwnerASC->GetAnimatingAbility())
			{
				if (bHasHitTarget == true)
				{
					if (URsGameplayAbility_Attack* AttackAbility = Cast<URsGameplayAbility_Attack>(CurrentAbility))
					{
						if (AttackAbility->bHasHitTarget != true)
						{
							return;
						}
					}
				}
				CurrentAbility->K2_CancelAbility();
				OwnerASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
			}
		}
	}
}
