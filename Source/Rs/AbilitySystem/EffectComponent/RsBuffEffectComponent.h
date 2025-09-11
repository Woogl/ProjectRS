// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"
#include "RsBuffEffectComponent.generated.h"

UENUM()
enum class ERsEffectTarget : uint8
{
	// Apply GE to self
	Source,
	// Apply GE to target
	Target,
};
/**
 * 
 */
UCLASS()
class RS_API URsBuffEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Stat"))
	FGameplayTag Stat;
	
	UPROPERTY(EditDefaultsOnly)
	ERsEffectTarget Target = ERsEffectTarget::Target;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Stat", ForceInlineRow))
	TMap<FGameplayTag, float> Coefficients;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (RowType = ""))
	// FDataTableRowHandle DataTableRow;
	
public:
	// Called when a Gameplay Effect is applied. This executes the OnApplication Gameplay Effects.
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
};
