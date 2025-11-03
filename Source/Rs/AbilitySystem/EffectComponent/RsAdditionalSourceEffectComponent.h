// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "RsAdditionalSourceEffectComponent.generated.h"

struct FGameplayEffectRemovalInfo;
struct FConditionalGameplayEffect;
/**
 * Apply additional Gameplay Effects to effect source (instigator)
 */
UCLASS(CollapseCategories)
class RS_API URsAdditionalSourceEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	/**
     * Called when a Gameplay Effect is Added to the ActiveGameplayEffectsContainer.  We register a callback to execute the OnComplete Gameplay Effects.
     */
	virtual bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& GEContainer, FActiveGameplayEffect& ActiveGE) const override;

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


protected:
	/**
	 * Whenever the ActiveGE gets removed, we want to apply the configured OnComplete GameplayEffects
	 */
	void OnActiveGameplayEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo, FActiveGameplayEffectsContainer* ActiveGEContainer) const;

public:
	// UPROPERTY(EditDefaultsOnly)
	// bool bApplyOnce = true;

	/** Other gameplay effects that will be applied to the source of this effect if the owning effect applies */
	UPROPERTY(EditDefaultsOnly, Category = OnApplication)
	TArray<TSubclassOf<UGameplayEffect>> OnApplicationGameplayEffects;

	/** Effects to apply when this effect completes, regardless of how it ends */
	UPROPERTY(EditDefaultsOnly, Category = OnComplete)
	TArray<TSubclassOf<UGameplayEffect>> OnCompleteAlways;

	/** Effects to apply when this effect expires naturally via its duration */
	UPROPERTY(EditDefaultsOnly, Category = OnComplete)
	TArray<TSubclassOf<UGameplayEffect>> OnCompleteNormal;

	/** Effects to apply when this effect is made to expire prematurely (e.g. via a forced removal, clear tags, etc.) */
	UPROPERTY(EditDefaultsOnly, Category = OnComplete)
	TArray<TSubclassOf<UGameplayEffect>> OnCompletePrematurely;
};
