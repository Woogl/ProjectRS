// Copyright 2025 Team BH.


#include "RsCanApplyEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

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
	// Check invincible for damage
	bool bInvincibleBlock = false;
	if (const FRsDamageTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsDamageTableRow>(GESpec))
	{
		float TargetINV = URsAbilitySystemLibrary::GetNumericAttributeByTag(ActiveGEContainer.Owner, RsGameplayTags::STAT_INV);
		bInvincibleBlock = TargetINV > Row->InvinciblePierce;
	}

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
			bStatBlock = StatValue == Value;
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
	
	return bInvincibleBlock || bStatBlock || bTagBlock;
}
