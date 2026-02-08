// Copyright 2025 Team BH.


#include "RsMaxStackEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

bool URsMaxStackEffectComponent::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	if (const FRsEffectTableRow* EffectTableRow = URsAbilitySystemGlobals::GetEffectTableRow<FRsEffectTableRow>(GESpec.GetContext()))
	{
		int32 MaxStack = EffectTableRow->FindValue<int32>(TEXT("MaxStack"), false);
		int32 CurrentStack = 0;
		
		FGameplayEffectQuery Query;
		Query.EffectDefinition = GESpec.Def.GetClass();
		TArray<FActiveGameplayEffectHandle> ActiveEffects = ActiveGEContainer.GetActiveEffects(Query);
		if (ActiveEffects.IsValidIndex(0))
		{
			FActiveGameplayEffectHandle ActiveEffect = ActiveEffects[0];
			CurrentStack = ActiveGEContainer.Owner->GetCurrentStackCount(ActiveEffect);
		}
		return CurrentStack < MaxStack;
	}
	
	return true;
}
