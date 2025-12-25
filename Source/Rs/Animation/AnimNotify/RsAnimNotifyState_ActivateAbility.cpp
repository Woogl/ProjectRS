// Copyright 2025 Team BH.


#include "RsAnimNotifyState_ActivateAbility.h"

#include "AbilitySystemComponent.h"

void URsAnimNotifyState_ActivateAbility::HandleConditionSatisfied()
{
	if (CurrentAbility.IsValid() && OwnerASC.IsValid() && AbilityTags.IsValid())
	{
		if (bCancelCurrentAbility == true)
		{
			CurrentAbility->K2_CancelAbility();
		}
		OwnerASC->TryActivateAbilitiesByTag(AbilityTags);
	}
	
	Super::HandleConditionSatisfied(); 
}
