// Copyright 2025 Team BH.


#include "RsCanApplyEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

bool URsCanApplyEffectComponent::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	if (ShouldImmunityBlock(ActiveGEContainer, GESpec))
	{
		ActiveGEContainer.Owner->OnImmunityBlockGameplayEffectDelegate.Broadcast(GESpec, nullptr);
		return false;
	}
	return Super::CanGameplayEffectApply(ActiveGEContainer, GESpec);
}

bool URsCanApplyEffectComponent::ShouldImmunityBlock(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	// Check stat
	bool bStatBlock = false;
	if (Comparision != ERsComparisionOperator::None)
	{
		float StatValue = URsAbilitySystemLibrary::GetNumericAttributeByTag(ActiveGEContainer.Owner, Stat);
		switch (Comparision)
		{
		case ERsComparisionOperator::Greater:
			bStatBlock = StatValue > Value;
			break;
		case ERsComparisionOperator::Equal:
			bStatBlock = FMath::IsNearlyEqual(StatValue, Value, UE_KINDA_SMALL_NUMBER);
			break;
		case ERsComparisionOperator::Less:
			bStatBlock = StatValue < Value;
			break;
		default:
			break;
		}
	}

	// Check tag requirements
	bool bTagBlock = false;
	if (!TagRequirements.IsEmpty())
	{
		FGameplayTagContainer Tags = ActiveGEContainer.Owner->GetOwnedGameplayTags();
		bTagBlock = TagRequirements.RequirementsMet(Tags);
	}
	
	return bStatBlock || bTagBlock;
}
