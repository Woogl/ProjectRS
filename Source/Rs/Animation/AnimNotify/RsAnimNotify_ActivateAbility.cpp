// Copyright 2024 Team BH.


#include "RsAnimNotify_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"

URsAnimNotify_ActivateAbility::URsAnimNotify_ActivateAbility()
{
}

void URsAnimNotify_ActivateAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!AbilityTags.IsValid() || !Owner)
	{
		return;
	}

	if (CurrentAbility.IsValid() && OwnerASC.IsValid())
	{
		if (bCancelCurrentAbility == true)
		{
			CurrentAbility->K2_CancelAbility();
		}
		OwnerASC->TryActivateAbilitiesByTag(AbilityTags);
	}
}
