// Copyright 2025 Team BH.


#include "RsCanApplyEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

bool URsCanApplyEffectComponent::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	if (ShouldImmunityBlock(ActiveGEContainer, GESpec))
	{
		if (bNotifyImmunityBlock)
		{
			ActiveGEContainer.Owner->OnImmunityBlockGameplayEffectDelegate.Broadcast(GESpec, nullptr);
		}
		return false;
	}
	return Super::CanGameplayEffectApply(ActiveGEContainer, GESpec);
}

bool URsCanApplyEffectComponent::ShouldImmunityBlock(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	bool bResult = false;
	if (Comparision != ERsComparisionOperator::None)
	{
		float StatValue = URsAbilitySystemLibrary::GetNumericAttributeByTag(ActiveGEContainer.Owner, Stat);
		switch (Comparision)
		{
		case ERsComparisionOperator::None:
			bResult = true;
			break;
		case ERsComparisionOperator::Greater:
			bResult = StatValue < Value;
			break;
		case ERsComparisionOperator::Equal:
			bResult = StatValue != Value;
			break;
		case ERsComparisionOperator::Less:
			bResult = StatValue > Value;
			break;
		}
	}

	if (!TagRequirements.IsEmpty())
	{
		FGameplayTagContainer Tags = ActiveGEContainer.Owner->GetOwnedGameplayTags();
		return bResult && !TagRequirements.RequirementsMet(Tags);
	}
	
	return bResult;
}
