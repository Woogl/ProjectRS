// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RsHealthDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsHealthDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	URsHealthDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	void HandleDotDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, float& OutDamage) const;
	
private:
	UPROPERTY()
	FName DataName_RemainingTime = TEXT("RemainingTime");
};
