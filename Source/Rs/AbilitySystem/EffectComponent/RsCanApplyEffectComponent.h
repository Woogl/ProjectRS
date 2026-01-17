// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayEffectTypes.h"
#include "RsCanApplyEffectComponent.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsCanApplyEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagRequirements TagRequirements;
	
	// 0 or minus means infinite stack count.
	UPROPERTY(EditDefaultsOnly)
	int32 MaxStack = 0;

public:
	/** Determine if we can apply this GameplayEffect or not */
	virtual bool CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const override;
	
	bool CheckTagRequirements(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const;
	bool CheckMaxStackCount(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const;
};
