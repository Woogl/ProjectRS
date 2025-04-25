// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCoefficientCalculation.h"
#include "RsHealthDamageCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsHealthDamageCalculation : public URsCoefficientCalculation
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
