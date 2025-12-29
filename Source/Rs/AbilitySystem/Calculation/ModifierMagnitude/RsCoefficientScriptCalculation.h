// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "RsCoefficientScriptCalculation.generated.h"

/**
 * Calculate magnitude based on captured attributes from source and target.
 */
UCLASS()
class RS_API URsCoefficientScriptCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	URsCoefficientScriptCalculation();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
