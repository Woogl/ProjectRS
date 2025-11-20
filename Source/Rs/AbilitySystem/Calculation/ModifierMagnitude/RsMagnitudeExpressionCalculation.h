// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCoefficientCalculationBase.h"
#include "RsMagnitudeExpressionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsMagnitudeExpressionCalculation : public URsCoefficientCalculationBase
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
