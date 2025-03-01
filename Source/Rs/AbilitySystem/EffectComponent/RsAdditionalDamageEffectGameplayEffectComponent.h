// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
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

#if WITH_EDITOR
	/**
	 * There are some fields that are incompatible with one another, so let's catch them during configuration phase. 
	 */
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
	
public:
	UPROPERTY(EditDefaultsOnly, Category = RsAdditionalDamageEffect)
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> AdditionalDamageEffectTable;
};
