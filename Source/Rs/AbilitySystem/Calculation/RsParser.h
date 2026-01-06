// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsParser.generated.h"

class URsCoefficientScriptExecution;
struct FGameplayEffectSpec;
class URsCoefficientScriptCalculation;
/**
 * 
 */
USTRUCT()
struct FRsParser
{
	GENERATED_BODY()
	
public:
	static float CoefficientScriptToFloat(const FString& Script, const FGameplayEffectSpec& Spec);
	
private:
	// Coefficient Script
	static TArray<FString> Tokenize(const FString& Script);
	static bool IsValidScript(const TArray<FString>& Tokens);
	static TArray<FString> ToPostfix(const TArray<FString>& Tokens);
	static float GetCapturedAttributeMagnitude(const FString& Token, const FGameplayEffectSpec& Spec);
	static FString CalculateOperation(const FString& Value1, const FString& Value2, const FString& Operator);
	static int32 GetPrecedence(const FString& Operator) ;
	static bool IsOperator(const FString& Operator);
	static bool IsOperator(const TCHAR& Operator);
};
