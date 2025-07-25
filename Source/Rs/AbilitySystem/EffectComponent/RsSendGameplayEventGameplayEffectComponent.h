// Copyright 2024 Team BH.

#pragma once


#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "RsSendGameplayEventGameplayEffectComponent.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Send Gameplay Event (Rs)")
class RS_API URsSendGameplayEventGameplayEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:

	/**
	 * Called when a Gameplay Effect is initially applied, or stacked.  GE's are 'applied' in both cases of duration or instant execution.  This call does not happen periodically, nor through replication.
	 * One should favor this function over OnActiveGameplayEffectAdded & OnGameplayEffectExecuted (but all multiple may be used depending on the case).
	 */
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

#if WITH_EDITOR
	/**
	 * Allow each Gameplay Effect Component to validate its own data.  Any warnings/errors will immediately show up in the Gameplay Effect when in Editor.
	 * The default implementation ensures we only have a single type of any given class.  Override this function to change that functionality and use Super::Super::IsDataValid if needed.
	 */
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

public:
	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTag EventTag;
};
