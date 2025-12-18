// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCoefficientScriptParser.generated.h"

class URsCoefficientScriptExecution;
struct FGameplayEffectSpec;
class URsCoefficientCalculationBase;
/**
 * 
 */
USTRUCT()
struct FRsCoefficientScriptParser
{
	GENERATED_BODY()
	
public:
	static float GetParseResult(const FString& Script, const FGameplayEffectSpec& Spec, const URsCoefficientCalculationBase* Calc);
	static float GetParseResult(const FString& Script, const FGameplayEffectSpec& Spec, const URsCoefficientScriptExecution* Exec);
	
private:
	static const FRsCoefficientScriptParser& Get();
	
	TArray<FString> Tokenize(const FString& Script) const;
	TArray<FString> ToPostfix(const TArray<FString>& Tokens) const;
	FString CalculateOperation(const FString& Value1, const FString& Value2, const FString& Operator) const;
	
	bool IsValid(const TArray<FString>& Tokens) const;
	int32 GetPrecedence(const FString& Operator) const;
	bool IsOperator(const FString& Operator) const;
	bool IsOperator(const TCHAR& Operator) const;
};
