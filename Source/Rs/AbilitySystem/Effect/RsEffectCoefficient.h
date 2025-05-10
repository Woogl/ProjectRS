// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RsEffectCoefficient.generated.h"

class UGameplayEffect;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsEffectCoefficient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Coefficient,Manual", ForceInlineRow))
	TMap<FGameplayTag, float> Coefficients;

	FRsEffectCoefficient() {}
	
	FRsEffectCoefficient(TSubclassOf<UGameplayEffect> InEffectClass, const TMap<FGameplayTag, float>& InCoefficients)
		: EffectClass(InEffectClass), Coefficients(InCoefficients) {}
};