// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RsStaggerExecCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsStaggerExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	URsStaggerExecCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
