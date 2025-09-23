// Copyright 2025 Team BH.


#include "RsCanApplyEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

URsCanApplyEffectComponent::URsCanApplyEffectComponent()
{
#if WITH_EDITORONLY_DATA
	EditorFriendlyName = FString();
#endif
}

bool URsCanApplyEffectComponent::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	if (ShouldImmunityBlock(ActiveGEContainer, GESpec))
	{
		// FActiveGameplayEffect DummyActiveEffect;
		// DummyActiveEffect.Spec = GESpec;
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
	if (Comparision != ERsStatComparision::None)
	{
		float StatValue = URsAbilitySystemLibrary::GetNumericAttributeByTag(ActiveGEContainer.Owner, Stat);
		switch (Comparision)
		{
		case ERsStatComparision::Greater:
			return StatValue < Value;
		case ERsStatComparision::Equal:
			return StatValue != Value;
		case ERsStatComparision::Less:
			return StatValue > Value;
		default:
			break;
		}
	}

	if (!TagRequirements.IsEmpty())
	{
		FGameplayTagContainer Tags = ActiveGEContainer.Owner->GetOwnedGameplayTags();
		return !TagRequirements.RequirementsMet(Tags);
	}
	
	return true;
}
