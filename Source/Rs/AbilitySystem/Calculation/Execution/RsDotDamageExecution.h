// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RsDotDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsDotDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	URsDotDamageExecution();
	
public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
