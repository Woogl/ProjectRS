// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsGameplayEffect.generated.h"

/**
 * Categorized gameplay effect for designer.
 */
UCLASS(Abstract)
class RS_API URsGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	URsGameplayEffect();
	
};
