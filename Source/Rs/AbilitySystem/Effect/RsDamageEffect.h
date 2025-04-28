// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsDamageEffect.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable)
class RS_API URsDamageEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
	virtual void PostInitProperties() override;
};
