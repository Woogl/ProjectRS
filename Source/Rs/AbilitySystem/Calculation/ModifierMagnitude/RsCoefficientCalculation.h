// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "RsCoefficientCalculation.generated.h"

/**
 * Calculate magnitude based on captured attributes from source and target.
 */

USTRUCT(BlueprintType)
struct FRsEffectMagnitudeExpressionParser
{
public:
	static float GetParseResult(const FString& Expression, const FGameplayEffectSpec& Spec, const URsCoefficientCalculation* Calc);
	
private:
	static TArray<FString> Tokenize(const FString& Expression);
	static TArray<FString> ToPostfix(const TArray<FString>& Tokens);
	static FString CalculateOperation(const FString& Value1, const FString& Value2, const FString& Operator);
	
	static bool IsValid(const TArray<FString>& Tokens);
	static int32 GetPrecedence(const FString& Operator);
	static int32 GetPrecedence(const TCHAR& Operator);
	static bool IsOperator(const FString& Operator);
	static bool IsOperator(const TCHAR& Operator);
};

UCLASS()
class RS_API URsCoefficientCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	URsCoefficientCalculation();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	float FindAttributeMagnitude(FGameplayTag Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters = FAggregatorEvaluateParameters()) const;
protected:
	void CaptureAttribute(FGameplayTag Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot);

	UPROPERTY()
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> CapturedAttributeDefinitions;
};

