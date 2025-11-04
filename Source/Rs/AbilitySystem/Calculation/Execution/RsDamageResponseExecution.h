// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RsDamageResponseExecution.generated.h"

/**
 * Trigger damage response to target, advantage to damage source.
 * It utilizes RsDamageEffectComponent.
 */
UCLASS()
class RS_API URsDamageResponseExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
