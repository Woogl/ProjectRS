// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "RsCrowdControlEffectComponent.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsCrowdControlEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Ability.HitReaction", ForceInlineRow))
	FGameplayTag EventTag;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (RowType = ""))
	// FDataTableRowHandle DataTableRow;
	
public:
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
};
