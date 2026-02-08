// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "RsMaxStackEffectComponent.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsMaxStackEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()
	
public:
	virtual bool CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const override;
};
