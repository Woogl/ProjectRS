// Copyright 2024 Team BH.


#include "RsAnimNotify_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"

URsAnimNotify_ActivateAbility::URsAnimNotify_ActivateAbility()
{
}

void URsAnimNotify_ActivateAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!ASC)
	{
		return;
	}

	if (bCancelCurrentAbility)
	{
		if (UGameplayAbility* CurrentAbility = ASC->GetAnimatingAbility())
		{
			CurrentAbility->K2_CancelAbility();
		}
	}
	
	if (AbilityTags.IsValid())
	{
		ASC->TryActivateAbilitiesByTag(AbilityTags);
	}
}
