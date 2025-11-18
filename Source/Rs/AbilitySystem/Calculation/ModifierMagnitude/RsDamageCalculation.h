// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCoefficientCalculationBase.h"
#include "RsDamageCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsHealthDamageCalculation : public URsCoefficientCalculationBase
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

UCLASS()
class RS_API URsStaggerDamageCalculation : public URsCoefficientCalculationBase
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
