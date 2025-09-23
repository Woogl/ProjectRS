// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsUnitEffect.generated.h"

/**
 * Categorized gameplay effect for RsInstantEffectComponent.
 */
UCLASS(Abstract)
class RS_API URsUnitEffect_Instant : public UGameplayEffect
{
	GENERATED_BODY()

public:
	URsUnitEffect_Instant();
};

/**
 * Categorized gameplay effect for RsBuffEffectComponent.
 */
UCLASS(Abstract)
class RS_API URsUnitEffect_Buff : public UGameplayEffect
{
	GENERATED_BODY()

public:
	URsUnitEffect_Buff();
};

/**
 * Categorized gameplay effect for RsPeriodicEffectComponent.
 */
UCLASS(Abstract)
class RS_API URsUnitEffect_Periodic : public UGameplayEffect
{
	GENERATED_BODY()

public:
	URsUnitEffect_Periodic();
};
