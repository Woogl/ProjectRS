// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RsStaggerDamageCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsStaggerDamageCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	URsStaggerDamageCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
