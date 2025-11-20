// Copyright 2024 Team BH.


#include "RsCoefficientCalculationBase.h"

#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/RsAbilitySystemSettings.h"

URsCoefficientCalculationBase::URsCoefficientCalculationBase()
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

float URsCoefficientCalculationBase::FindAttributeMagnitude(FGameplayTag Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters) const
{
	float OutMagnitude = 0.f;
	if (CapturedAttributeDefinitions.Contains(Key))
	{
		GetCapturedAttributeMagnitude(CapturedAttributeDefinitions[Key], Spec, EvaluationParameters, OutMagnitude);
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("Cannot find [ %s ] attribute"), *Key.ToString());
	}
	return OutMagnitude;
}

void URsCoefficientCalculationBase::CaptureAttribute(FGameplayTag Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot)
{
	FGameplayEffectAttributeCaptureDefinition Definition;
	Definition.AttributeToCapture = Attribute;
	Definition.AttributeSource = SourceOrTarget;
	Definition.bSnapshot = bSnapShot;
	RelevantAttributesToCapture.Add(Definition);
	CapturedAttributeDefinitions.Add(Key, Definition);
}

float FRsExpressionParser::GetParseResult(const FString& Expression, const FGameplayEffectSpec& Spec, const URsCoefficientCalculationBase* Calc)
{
	const FRsExpressionParser Parser = Get();
	
	const TArray<FString> Tokens = Parser.Tokenize(Expression);

	if (!Parser.IsValid(Tokens))
	{
		UE_LOG(RsLog, Error, TEXT("Invalid Magnitude Expression [ %s ] detected in [ %s ]."), *Expression, *Spec.ToSimpleString());
		return 0.f;
	}

	const TArray<FString> PostfixTokens = Parser.ToPostfix(Tokens);
	TArray<FString> Stack;

	for (const FString& Token : PostfixTokens)
	{
		if (!Parser.IsOperator(Token))
		{
			if (Token.IsNumeric())
			{
				Stack.Push(Token);
			}
			else
			{
				FAggregatorEvaluateParameters EvaluationParameters;
				float AttributeValue = Calc->FindAttributeMagnitude(FGameplayTag::RequestGameplayTag(FName(Token)),Spec, EvaluationParameters);
				Stack.Push(FString::Printf(TEXT("%f"), AttributeValue));
			}
		}
		else
		{
			FString Value2 = Stack.Pop();
			FString Value1 = Stack.Pop();

			Stack.Push(Parser.CalculateOperation(Value1, Value2, Token));
		}
	}

	return FCString::Atof(*Stack.Pop());
}

const FRsExpressionParser& FRsExpressionParser::Get()
{
	static FRsExpressionParser Parser;
	return Parser;
}

TArray<FString> FRsExpressionParser::Tokenize(const FString& Expression) const
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

TArray<FString> FRsExpressionParser::ToPostfix(const TArray<FString>& Tokens) const
{
	TArray<FString> Output;
	TArray<FString> Stack;
    
	for (const FString& Token : Tokens)
	{
		if (Token == TEXT("("))
		{
			Stack.Push(Token);
		}
		else if (Token == TEXT(")"))
		{
			while (!Stack.IsEmpty() && Stack.Top() != TEXT("("))
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

FString FRsExpressionParser::CalculateOperation(const FString& Value1, const FString& Value2, const FString& Operator) const
{
	float Value = 0.f;
	
	if (Operator == TEXT("+"))
	{
		Value = FCString::Atof(*Value1) + FCString::Atof(*Value2);
	}
	if (Operator == TEXT("-"))
	{
		Value = FCString::Atof(*Value1) - FCString::Atof(*Value2);
	}
	if (Operator == TEXT("*"))
	{
		Value = FCString::Atof(*Value1) * FCString::Atof(*Value2);
	}
	if (Operator == TEXT("/"))
	{
		Value = FCString::Atof(*Value1) / FCString::Atof(*Value2);
	}

	return FString::Printf(TEXT("%f"), Value);
}


bool FRsExpressionParser::IsValid(const TArray<FString>& Tokens) const
{
	if(Tokens.IsEmpty())
	{
		UE_LOG(RsLog, Error, TEXT("Invalid Expression detected."));
		return false;		
	}
	
	int BracketCount = 0;
	for (const FString& Token : Tokens)
	{
		if (Token == TEXT("("))
		{
			++BracketCount;
		}
		else if (Token == TEXT(")"))
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

int32 FRsExpressionParser::GetPrecedence(const FString& Operator) const
{
	if (Operator == TEXT("*") || Operator == TEXT("/"))
	{
		return 2;
	}
	if (Operator == TEXT("+") || Operator == TEXT("-"))
	{
		return 1;
	}
	return 0;
}

int32 FRsExpressionParser::GetPrecedence(const TCHAR& Operator) const
{
	if (Operator == '*' || Operator == '/')
	{
		return 2;
	}
	if (Operator == '+' || Operator == '-')
	{
		return 1;
	}
	return 0;
}

bool FRsExpressionParser::IsOperator(const FString& Operator) const
{
	if (Operator == TEXT("+") || Operator == TEXT("-") || Operator == TEXT("*") || Operator == TEXT("/"))
	{
		return true;
	}
	return false;
}

bool FRsExpressionParser::IsOperator(const TCHAR& Operator) const
{
	if (Operator == '+' || Operator == '-' || Operator == '*' || Operator == '/')
	{
		return true;
	}
	return false;
}