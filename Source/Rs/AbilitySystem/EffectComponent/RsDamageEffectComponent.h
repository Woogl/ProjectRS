// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "RsDamageEffectComponent.generated.h"

struct FRsDamageTableRow;
/**
 * It contains datas that make damage.
 * AssetTags + CanApply + ModifierData + HitReaction + HitStop
 */
UCLASS()
class RS_API URsDamageEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	URsDamageEffectComponent();
	
	// Handle DamageTags
	virtual void OnGameplayEffectChanged() override;

	// Handle HitReaction, HitStops, ManaGain, UltimateGain, AdditionalEffects
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
};
