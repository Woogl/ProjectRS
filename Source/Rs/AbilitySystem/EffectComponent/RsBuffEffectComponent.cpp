// Copyright 2025 Team BH.


#include "RsBuffEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"
#include "Rs/AbilitySystem/Effect/RsUnitEffect.h"

void URsBuffEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	if (!ensureMsgf(ActiveGEContainer.Owner, TEXT("OnGameplayEffectApplied is passed an ActiveGEContainer which lives within an ASC but that ASC was somehow null")))
	{
		return;
	}
	
	FRsEffectCoefficient EffectCoefficient(Effect, Coefficients);
	UAbilitySystemComponent* SourceASC = GESpec.GetContext().GetInstigatorAbilitySystemComponent();

	FGameplayEffectSpecHandle EffectHandle = URsAbilitySystemLibrary::MakeEffectSpecCoefficient(SourceASC, EffectCoefficient, SourceASC->MakeEffectContext());
	EffectHandle.Data->Duration = GESpec.Duration;

	URsAbilitySystemLibrary::ApplyEffectSpecCoefficient(SourceASC, ActiveGEContainer.Owner, EffectHandle);
}
