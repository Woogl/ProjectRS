// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RsCoefficientScriptExecution.generated.h"

struct FRsEffectTableRow;

/**
 * NOTE: This class cannot be used with GameplayEffects that have a duration but no period (Period = 0).
 */
UCLASS()
class RS_API URsCoefficientScriptExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	URsCoefficientScriptExecution();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:
	void ApplyParseResult(const FRsEffectTableRow* Row, FName TableKey, const FGameplayAttribute& TargetAttribute, const FGameplayEffectSpec& Spec, FGameplayEffectCustomExecutionOutput& Output) const;
};
