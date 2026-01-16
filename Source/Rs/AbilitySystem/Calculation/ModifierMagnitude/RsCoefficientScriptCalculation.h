// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "RsCoefficientScriptCalculation.generated.h"

/**
 * Calculate magnitude based on captured attributes from source and target.
 */
UCLASS(Abstract)
class RS_API URsCoefficientScriptCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	URsCoefficientScriptCalculation();
	
	UPROPERTY(EditDefaultsOnly, Category="RS")
	FName TableKey = NAME_None;
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
