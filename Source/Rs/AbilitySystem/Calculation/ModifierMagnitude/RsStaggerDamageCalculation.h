// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCoefficientCalculation.h"
#include "RsStaggerDamageCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsStaggerDamageCalculation : public URsCoefficientCalculation
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
