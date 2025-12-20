// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCoefficientScriptCalculation.h"
#include "RsDamageCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsHealthDamageCalculation : public URsCoefficientScriptCalculation
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

UCLASS()
class RS_API URsStaggerDamageCalculation : public URsCoefficientScriptCalculation
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
