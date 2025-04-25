// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "RsCoefficientCalculation.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsCoefficientCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	URsCoefficientCalculation();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	void CaptureAttribute(FGameplayTag Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot);
	float FindAttributeMagnitude(FGameplayTag Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters = FAggregatorEvaluateParameters()) const;
	
private:
	UPROPERTY()
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> CapturedAttributeDefinitions;
};

