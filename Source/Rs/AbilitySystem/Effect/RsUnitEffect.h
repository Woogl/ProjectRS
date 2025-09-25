// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsUnitEffect.generated.h"

UCLASS(Abstract, NotBlueprintable)
class RS_API URsUnitEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	URsUnitEffect();
};

/**
 * Categorized gameplay effect for RsInstantEffectComponent.
 */
UCLASS(Abstract, Blueprintable)
class RS_API URsUnitEffect_Instant : public URsUnitEffect
{
	GENERATED_BODY()

public:
	URsUnitEffect_Instant();
};

/**
 * Categorized gameplay effect for RsBuffEffectComponent.
 */
UCLASS(Abstract, Blueprintable)
class RS_API URsUnitEffect_Buff : public URsUnitEffect
{
	GENERATED_BODY()

public:
	URsUnitEffect_Buff();
};

/**
 * Categorized gameplay effect for RsPeriodicEffectComponent.
 */
UCLASS(Abstract, Blueprintable)
class RS_API URsUnitEffect_Periodic : public URsUnitEffect
{
	GENERATED_BODY()

public:
	URsUnitEffect_Periodic();
};
