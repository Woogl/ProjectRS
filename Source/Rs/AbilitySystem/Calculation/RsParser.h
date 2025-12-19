// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsParser.generated.h"

class URsCoefficientScriptExecution;
struct FGameplayEffectSpec;
class URsCoefficientCalculationBase;
/**
 * 
 */
USTRUCT()
struct FRsParser
{
	GENERATED_BODY()
	
public:
	static float CoefficientScriptToFloat(const FString& Script, const FGameplayEffectSpec& Spec, const URsCoefficientCalculationBase* Calc);
	static float CoefficientScriptToFloat(const FString& Script, const FGameplayEffectSpec& Spec, const URsCoefficientScriptExecution* Exec);
	
private:
	static const FRsParser& Get();
	
	// Coefficient Script
	TArray<FString> Tokenize(const FString& Script) const;
	TArray<FString> ToPostfix(const TArray<FString>& Tokens) const;
	FString CalculateOperation(const FString& Value1, const FString& Value2, const FString& Operator) const;
	bool IsValidScript(const TArray<FString>& Tokens) const;
	int32 GetPrecedence(const FString& Operator) const;
	bool IsOperator(const FString& Operator) const;
	bool IsOperator(const TCHAR& Operator) const;
};
