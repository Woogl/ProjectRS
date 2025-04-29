// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsDamageEffect.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, Abstract)
class RS_API URsDamageEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
};
