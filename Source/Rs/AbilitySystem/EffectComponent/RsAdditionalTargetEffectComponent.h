// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponents/AdditionalEffectsGameplayEffectComponent.h"
#include "RsAdditionalTargetEffectComponent.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAdditionalTargetEffectComponent : public UAdditionalEffectsGameplayEffectComponent
{
	GENERATED_BODY()

public:
	// It support RsEffectTable.
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
};
