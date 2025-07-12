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
	float FindAttributeMagnitude(FGameplayTag Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters = FAggregatorEvaluateParameters()) const;
	
private:
	void CaptureAttribute(FGameplayTag Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot);
	
	UPROPERTY()
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> CapturedAttributeDefinitions;
};

