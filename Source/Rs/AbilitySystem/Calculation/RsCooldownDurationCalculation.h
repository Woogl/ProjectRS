// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "RsCooldownDurationCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsCooldownDurationCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
