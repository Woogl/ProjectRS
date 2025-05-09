// Copyright 2024 Team BH.


#include "RsAnimNotify_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/System/RsGenericContainer.h"

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
				if (bHasHitTarget == true)
				{
					if (URsGameplayAbility* RsAbility = Cast<URsGameplayAbility>(CurrentAbility))
					{
						if (RsAbility->StatesContainer->GetValue<bool>(FName("HasHitTarget")) == false)
						{
							return;
						}
					}
				}
				if (bCancelCurrentAbility == true)
				{
					CurrentAbility->K2_CancelAbility();
				}
				OwnerASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
			}
		}
	}
}
