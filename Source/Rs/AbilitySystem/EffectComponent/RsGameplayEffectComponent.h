// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayEffectTypes.h"
#include "RsGameplayEffectComponent.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsGameplayEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	/**
	 * Called when a Gameplay Effect is Added to the ActiveGameplayEffectsContainer.  GE's are added to that container when they have duration (or are predicting locally).
	 * Note: This also occurs as a result of replication (e.g. when the server replicates a GE to the owning client -- including the 'duplicate' GE after a prediction).
	 * Return if the effect should remain active, or false to inhibit.  Note: Inhibit does not remove the effect (it remains added but dormant, waiting to uninhibit).
	 */
	virtual bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const override;

	/**
 * Called when a Gameplay Effect is initially applied, or stacked.  GE's are 'applied' in both cases of duration or instant execution.  This call does not happen periodically, nor through replication.
 * One should favor this function over OnActiveGameplayEffectAdded & OnGameplayEffectExecuted (but all multiple may be used depending on the case).
 */
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

	void OnActiveGameplayEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo, FActiveGameplayEffectsContainer* ActiveGEContainer) const;
};
