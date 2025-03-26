// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RsHealthDamageCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsHealthDamageCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	URsHealthDamageCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
