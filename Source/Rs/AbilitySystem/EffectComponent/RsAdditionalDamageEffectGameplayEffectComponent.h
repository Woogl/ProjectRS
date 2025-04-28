// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "RsAdditionalDamageEffectGameplayEffectComponent.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Apply Additional Damage Effect (Rs)")
class RS_API URsAdditionalDamageEffectGameplayEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:

	/**
	 * Called when a Gameplay Effect is applied.  This executes the OnApplication Gameplay Effects.
	 */
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
};
