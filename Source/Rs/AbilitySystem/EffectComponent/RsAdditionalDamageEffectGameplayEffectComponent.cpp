// Copyright 2024 Team BH.


#include "RsAdditionalDamageEffectGameplayEffectComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "URsAdditionalDamageEffectGameplayEffectComponent"


void URsAdditionalDamageEffectGameplayEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);
	for (FGameplayEffectSpecHandle AdditionalEffectSpec : GESpec.TargetEffectSpecs)
	{
		ActiveGEContainer.Owner->ApplyGameplayEffectSpecToSelf(*AdditionalEffectSpec.Data);
	}
}

#undef LOCTEXT_NAMESPACE

