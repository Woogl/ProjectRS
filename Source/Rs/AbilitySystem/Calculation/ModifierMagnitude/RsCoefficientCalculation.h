// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "RsCoefficientCalculation.generated.h"

/**
 * Calculate magnitude based on captured attributes from source and target.
 */
UCLASS()
class RS_API URsCoefficientCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	URsCoefficientCalculation();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	void CaptureAttribute(FName Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot);
	float FindAttributeMagnitude(FName Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters = FAggregatorEvaluateParameters()) const;
	FName ChopLastDot(const FName& InName) const;

	UPROPERTY()
	TMap<FName, FGameplayEffectAttributeCaptureDefinition> CapturedAttributeDefinitions;
};

