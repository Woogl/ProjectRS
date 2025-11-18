// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "RsCoefficientCalculationBase.generated.h"

/**
 * Calculate magnitude based on captured attributes from source and target.
 */
UCLASS(Abstract)
class RS_API URsCoefficientCalculationBase : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	URsCoefficientCalculationBase();
	
	// virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
	float FindAttributeMagnitude(FGameplayTag Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters = FAggregatorEvaluateParameters()) const;

protected:
	void CaptureAttribute(FGameplayTag Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot);

	UPROPERTY()
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> CapturedAttributeDefinitions;
};


USTRUCT()
struct FRsExpressionParser
{
	GENERATED_BODY()
	
public:
	static float GetParseResult(const FString& Expression, const FGameplayEffectSpec& Spec, const URsCoefficientCalculationBase* Calc);
	
private:
	static const FRsExpressionParser& Get();
	
	TArray<FString> Tokenize(const FString& Expression) const;
	TArray<FString> ToPostfix(const TArray<FString>& Tokens) const;
	FString CalculateOperation(const FString& Value1, const FString& Value2, const FString& Operator) const;
	
	bool IsValid(const TArray<FString>& Tokens) const;
	int32 GetPrecedence(const FString& Operator) const;
	int32 GetPrecedence(const TCHAR& Operator) const;
	bool IsOperator(const FString& Operator) const;
	bool IsOperator(const TCHAR& Operator) const;
};