// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "RsCrowdControlEffectComponent.generated.h"

enum class ERsComparisionOperator : uint8;
/**
 * 
 */
UCLASS()
class RS_API URsCrowdControlEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Ability.HitReaction", ForceInlineRow))
	FGameplayTag EventTag;

	// Set "True" when you want to check target's Super Armor.
	UPROPERTY(EditDefaultsOnly)
	bool bUseInternalCanApply = false;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bUseInternalCanApply", EditConditionHides, Categories = "Stat"))
	FGameplayTag Stat;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bUseInternalCanApply", EditConditionHides))
	ERsComparisionOperator Comparision;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bUseInternalCanApply", EditConditionHides))
	float Value;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bUseInternalCanApply", EditConditionHides))
	FGameplayTagRequirements TagRequirements;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (RowType = ""))
	// FDataTableRowHandle DataTableRow;
	
public:
	URsCrowdControlEffectComponent();
	
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

private:
	bool InternalCanApply(const FActiveGameplayEffectsContainer& ActiveGEContainer) const;
};
