// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "RsHitStopEffectComponent.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsHitStopEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SourceHitStopTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetHitStopTime = 0.2f;
};
