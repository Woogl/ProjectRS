// Copyright 2024 Team BH.


#include "RsAnimNotify_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"

URsAnimNotify_ActivateAbility::URsAnimNotify_ActivateAbility()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_ActivateAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			if (UGameplayAbility* CurrentAbility = OwnerASC->GetAnimatingAbility())
			{
				if (bCancelCurrentAbility == true)
				{
					CurrentAbility->K2_CancelAbility();
				}
				OwnerASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
			}
		}
	}
}
