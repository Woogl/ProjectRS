// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "RsPeriodicEffectComponent.generated.h"

class URsUnitEffect_Periodic;
/**
 * 
 */
UCLASS()
class RS_API URsPeriodicEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URsUnitEffect_Periodic> Effect;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Coefficient", ForceInlineRow))
	TMap<FGameplayTag, float> Coefficients;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (RowType = ""))
	// FDataTableRowHandle DataTableRow;
	
public:
	// Called when a Gameplay Effect is applied. This executes the OnApplication Gameplay Effects.
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
};
