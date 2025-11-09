// Copyright 2024 Team BH.


#include "RsCoefficientCalculation.h"

#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/RsAbilitySystemSettings.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

float FRsEffectMagnitudeExpressionParser::GetParseResult(const FString& Expression, const FGameplayEffectSpec& Spec, const URsCoefficientCalculation* Calc)
{
	const TArray<FString> Tokens = Tokenize(Expression);

	if (!IsValid(Tokens))
	{
		UE_LOG(RsLog, Error, TEXT("Invalid Magnitude Expression [ %s ] detected in [ %s ]."), *Expression, *Spec.Def->GetFName().ToString());
		return 0.0f;
	}

	const TArray<FString> PostfixTokens = ToPostfix(Tokens);
	TArray<FString> Stack;

	for (const FString& Token : PostfixTokens)
	{
		if (!IsOperator(Token))
		{
			if (Token.IsNumeric())
			{
				Stack.Push(Token);
			}
			else
			{
				FAggregatorEvaluateParameters EvaluationParameters;
				float AttributeValue = Calc->FindAttributeMagnitude(FGameplayTag::RequestGameplayTag(FName(Token)),Spec, EvaluationParameters);
				Stack.Push(FString::Printf(AttributeValue));
			}
		}
		else
		{
			FString Value2 = Stack.Pop();
			FString Value1 = Stack.Pop();

			Stack.Push(CalculateOperation(Value1, Value2, Token));
		}
	}

	return FCString::Atof(*Stack.Pop());
}

TArray<FString> FRsEffectMagnitudeExpressionParser::Tokenize(const FString& Expression)
{
	TArray<FString> Tokens;
	FString TempToken;

	for (TCHAR C : Expression)
	{
		if (FChar::IsWhitespace(C)) continue;

		if (IsOperator(C) || C == '(' || C == ')')
		{
			if (TempToken.Len() > 0)
			{
				Tokens.Add(TempToken);
				TempToken.Empty();
			}
			Tokens.Add(FString(1, &C));
		}
		else
		{
			TempToken.AppendChar(C);
		}
	}

	if (TempToken.Len() > 0)
	{
		Tokens.Add(TempToken);
	}
	
	return Tokens;
}

TArray<FString> FRsEffectMagnitudeExpressionParser::ToPostfix(const TArray<FString>& Tokens)
{
	TArray<FString> Output;
	TArray<FString> Stack;
    
	for (const FString& Token : Tokens)
	{
		if (Token == "(")
		{
			Stack.Push(Token);
		}
		else if (Token == ")")
		{
			while (!Stack.IsEmpty() && Stack.Top() != "(")
			{
				Output.Add(Stack.Top());
				Stack.Pop();
			}
			if (!Stack.IsEmpty()) // remove '('
			{
				Stack.Pop();
			}
		}
		else if (IsOperator(Token))
		{
			while (!Stack.IsEmpty() && IsOperator(Stack.Top()) && GetPrecedence(Stack.Top()) >= GetPrecedence(Token))
			{
				Output.Add(Stack.Top());
				Stack.Pop();
			}
			Stack.Push(Token);
		}
		else
		{
			Output.Add(Token);
		}
	}

	while (!Stack.IsEmpty())
	{
		Output.Add(Stack.Top());
		Stack.Pop();
	}

	return Output;
}

FString FRsEffectMagnitudeExpressionParser::CalculateOperation(const FString& Value1, const FString& Value2, const FString& Operator)
{
	float Value = 0.f;
	
	if (Operator == "+") Value = FCString::Atof(*Value1) + FCString::Atof(*Value2);
	if (Operator == "-") Value = FCString::Atof(*Value1) - FCString::Atof(*Value2);
	if (Operator == "*") Value = FCString::Atof(*Value1) * FCString::Atof(*Value2);
	if (Operator == "/") Value = FCString::Atof(*Value1) / FCString::Atof(*Value2);

	return FString::Printf(Value);
}


bool FRsEffectMagnitudeExpressionParser::IsValid(const TArray<FString>& Tokens)
{
	if(Tokens.IsEmpty())
	{
		UE_LOG(RsLog, Error, TEXT("Invalid Expression detected."));
		return false;		
	}
	
	int BracketCount = 0;
	for (const FString& Token : Tokens)
	{
		if (Token == "(")
		{
			++BracketCount;
		}
		else if (Token == ")")
		{
			--BracketCount;
		}
	}

	if (BracketCount != 0)
	{
		UE_LOG(RsLog, Error, TEXT("Invalid Bracket Count detected."));
		return false;
	}
	
	return true;
}

int32 FRsEffectMagnitudeExpressionParser::GetPrecedence(const FString& Operator)
{
	if (Operator == "*" || Operator == "/") return 2;
	if (Operator == "+" || Operator == "-") return 1;
	return 0;
}

int32 FRsEffectMagnitudeExpressionParser::GetPrecedence(const TCHAR& Operator)
{
	if (Operator == '*' || Operator == '/') return 2;
	if (Operator == '+' || Operator == '-') return 1;
	return 0;
}

bool FRsEffectMagnitudeExpressionParser::IsOperator(const FString& Operator)
{
	if (Operator == "+" || Operator == "-" || Operator == "*" || Operator == "/") return true;
	return false;
}

bool FRsEffectMagnitudeExpressionParser::IsOperator(const TCHAR& Operator)
{
	if (Operator == '+' || Operator == '-' || Operator == '*' || Operator == '/') return true;
	return false;
}

URsCoefficientCalculation::URsCoefficientCalculation()
{
	// Capture every attribute of source and target
	// NOTE: Attributes to be captured must exist in the source and target!
    for (const TTuple<FGameplayTag, FGameplayAttribute>& CoefficientTag : URsAbilitySystemSettings::Get().Coefficients)
    {
	    FString CoeffString = CoefficientTag.Key.ToString();
    	FGameplayTag SourceTag = FGameplayTag::RequestGameplayTag(FName(CoeffString + TEXT(".Source")));
    	FGameplayTag TargetTag = FGameplayTag::RequestGameplayTag(FName(CoeffString + TEXT(".Target")));
    	
    	CaptureAttribute(SourceTag, CoefficientTag.Value, EGameplayEffectAttributeCaptureSource::Source, true);
    	CaptureAttribute(TargetTag, CoefficientTag.Value, EGameplayEffectAttributeCaptureSource::Target, false);
	}
}

float URsCoefficientCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	const FRsEffectTableRow* TableRow = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsEffectTableRow>(Spec);
	return FRsEffectMagnitudeExpressionParser::GetParseResult(TableRow->MagnitudeExpression, Spec, this);
}

float URsCoefficientCalculation::FindAttributeMagnitude(FGameplayTag Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters) const
{
	float OutMagnitude = 0.f;
	if (CapturedAttributeDefinitions.Contains(Key))
	{
		GetCapturedAttributeMagnitude(CapturedAttributeDefinitions[Key], Spec, EvaluationParameters, OutMagnitude);
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("Cannot find {%s} attribute"), *Key.ToString());
	}
	return OutMagnitude;
}

void URsCoefficientCalculation::CaptureAttribute(FGameplayTag Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot)
{
	FGameplayEffectAttributeCaptureDefinition Definition;
	Definition.AttributeToCapture = Attribute;
	Definition.AttributeSource = SourceOrTarget;
	Definition.bSnapshot = bSnapShot;
	RelevantAttributesToCapture.Add(Definition);
	CapturedAttributeDefinitions.Add(Key, Definition);
}
