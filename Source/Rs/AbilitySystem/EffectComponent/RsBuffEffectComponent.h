// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "Rs/AbilitySystem/Effect/RsEffectTypes.h"
#include "RsBuffEffectComponent.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsBuffEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Stat"))
	FGameplayTag StatTag;
	
	UPROPERTY(EditDefaultsOnly)
	ERsEffectTarget TargetType = ERsEffectTarget::Target;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Coefficient,Manual", ForceInlineRow))
	TMap<FGameplayTag, float> Coefficients;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (RowType = ""))
	// FDataTableRowHandle DataTableRow;
	
public:
	// Called when a Gameplay Effect is applied. This executes the OnApplication Gameplay Effects.
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
};
