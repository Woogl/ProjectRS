// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsEffectCoefficient.h"
#include "RsBuffEffect.generated.h"

USTRUCT(BlueprintType)
struct FRsBuffCoefficient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class URsBuffEffect> BuffClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient,Manual.Magnitude", ForceInlineRow))
	TMap<FGameplayTag, float> Coefficients;

	FRsEffectCoefficient ToRsEffectCoefficient(float Duration) const;
};

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

	UFUNCTION(BlueprintCallable)
	static FRsEffectCoefficient ToRsEffectCoefficient(const FRsBuffCoefficient& BuffCoefficient, float Duration);
};
