// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RsHealthDamageExecCalculation.generated.h"

/**
 * DEPRECATED: It needs RsHealthSet::BaseDamage.
 */
UCLASS(Deprecated)
class RS_API UDEPRECATED_RsHealthDamageExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UDEPRECATED_RsHealthDamageExecCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
