// Copyright 2025 Team BH.


#include "RsCanApplyEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"

bool URsCanApplyEffectComponent::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	bool bParentPass = Super::CanGameplayEffectApply(ActiveGEContainer, GESpec);
	bool bTagPass = CheckTagRequirements(ActiveGEContainer, GESpec);
	bool bStackPass = CheckMaxStackCount(ActiveGEContainer, GESpec);
	
	return bParentPass && bTagPass && bStackPass;
}

bool URsCanApplyEffectComponent::CheckTagRequirements(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	UAbilitySystemComponent* TargetASC = ActiveGEContainer.Owner;
	if (!TargetASC)
	{
		return false;
	}
	
	if (!TagRequirements.IsEmpty())
	{
		FGameplayTagContainer Tags = ActiveGEContainer.Owner->GetOwnedGameplayTags();
		return TagRequirements.RequirementsMet(Tags);
	}
	
	return true;
}

bool URsCanApplyEffectComponent::CheckMaxStackCount(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	UAbilitySystemComponent* TargetASC = ActiveGEContainer.Owner;
	if (!TargetASC)
	{
		return false;
	}

	int32 LocalMaxStack = MaxStack;
	if (const FRsEffectTableRow* EffectTableRow = URsAbilitySystemGlobals::GetEffectTableRow<FRsEffectTableRow>(GESpec.GetContext()))
	{
		LocalMaxStack = EffectTableRow->FindValue<int32>(TEXT("MaxStack"), false);
	}
	
	int32 CurrentStack = 0;
	FGameplayEffectQuery Query;
	Query.EffectDefinition = GESpec.Def.GetClass();
	TArray<FActiveGameplayEffectHandle> ActiveEffects = ActiveGEContainer.GetActiveEffects(Query);
	if (ActiveEffects.IsValidIndex(0))
	{
		FActiveGameplayEffectHandle ActiveEffect = ActiveEffects[0];
		CurrentStack = TargetASC->GetCurrentStackCount(ActiveEffect);
	}
	return CurrentStack < LocalMaxStack;
}
