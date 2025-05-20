// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsBuffEffect.generated.h"

/**
 * Categorized gameplay effect used by AnimNotify. It must have a duration.
 * Used in: AN_BuffToSelf, AN_BuffToTarget, ANS_BuffToSelf, ANS_BuffToTarget
 */
UCLASS(Abstract)
class RS_API URsBuffEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	URsBuffEffect();
};
