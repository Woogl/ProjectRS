// Copyright 2024 Team BH.


#include "RsAnimNotify_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/Condition/RsCondition.h"

URsAnimNotify_ActivateAbility::URsAnimNotify_ActivateAbility()
{
}

void URsAnimNotify_ActivateAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
	if (Condition && !Condition->IsSatisfied(Owner))
	{
		return;
	}
	
	if (!AbilityTags.IsValid())
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
