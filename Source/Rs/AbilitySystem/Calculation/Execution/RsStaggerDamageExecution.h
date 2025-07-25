// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RsStaggerDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsStaggerDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	URsStaggerDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	UPROPERTY()
	FName DataName_RemainingTime = TEXT("RemainingTime");
};
