// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "RsAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	/** Should allocate a project specific GameplayEffectContext struct. Caller is responsible for deallocation */
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
