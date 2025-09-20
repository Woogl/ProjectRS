// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectAttributeCaptureDefinition.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RsEffectCoefficient.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FRsStatCoefficient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Stat,Manual", ForceInlineRow))
	TMap<FGameplayTag, float> StatCoefficients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameplayEffectAttributeCaptureSource CaptureSource;
};

USTRUCT(BlueprintType)
struct FRsEffectCoefficient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRsStatCoefficient> Coefficients;

	bool IsValid() const;

	FRsEffectCoefficient() {};
	FRsEffectCoefficient(TSubclassOf<UGameplayEffect> EffectClass, const TArray<FRsStatCoefficient>& Coefficients);
	FRsEffectCoefficient(TSubclassOf<UGameplayEffect> EffectClass, float Magnitude);
};