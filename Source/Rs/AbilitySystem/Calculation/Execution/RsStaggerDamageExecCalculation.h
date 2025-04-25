// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RsStaggerDamageExecCalculation.generated.h"

/**
 * DEPRECATED: It needs RsStaggerSet::BaseDamage.
 */
UCLASS(Deprecated)
class RS_API UDEPRECATED_RsStaggerDamageExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UDEPRECATED_RsStaggerDamageExecCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
