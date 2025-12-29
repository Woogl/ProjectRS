// Copyright 2025 Team BH.


#include "RsParser.h"

#include "GameplayTagContainer.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"

static const FRsParser& Get()
{
	static FRsParser ParserStatics;
	return ParserStatics;
}

float FRsParser::CoefficientScriptToFloat(const FString& Script, const FGameplayEffectSpec& Spec)
{
	if (Script.IsEmpty())
	{
		return 0.f;
	}
	
	if (Script.IsNumeric())
	{
		return FCString::Atof(*Script);
	}
	
	const FRsParser Parser = Get();
	
	const TArray<FString> Tokens = Parser.Tokenize(Script);

	if (!Parser.IsValidScript(Tokens))
	{
		UE_LOG(LogRsAbility, Error, TEXT("Invalid script [%s] detected in [%s]."), *Script, *Spec.ToSimpleString());
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
				const float AttributeValue = Parser.GetCapturedAttributeMagnitude(Token, Spec);
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

	float Result = FCString::Atof(*Stack.Pop());
	return Result;
}

TArray<FString> FRsParser::Tokenize(const FString& Script) const
{
	TArray<FString> OutTokens;
	FString LocalToken;

	for (TCHAR C : Script)
	{
		if (FChar::IsWhitespace(C))
		{
			continue;
		}

		if (IsOperator(C) || C == '(' || C == ')')
		{
			if (LocalToken.Len() > 0)
			{
				OutTokens.Add(LocalToken);
				LocalToken.Empty();
			}
			OutTokens.Add(FString(1, &C));
		}
		else
		{
			LocalToken.AppendChar(C);
		}
	}

	if (LocalToken.Len() > 0)
	{
		OutTokens.Add(LocalToken);
	}
	
	return OutTokens;
}

bool FRsParser::IsValidScript(const TArray<FString>& Tokens) const
{
	if (Tokens.IsEmpty())
	{
		UE_LOG(LogRsAbility, Error, TEXT("Invalid Script: Empty token."));
		return false;		
	}
	
	int32 BracketCount = 0;
	for (const FString& Token : Tokens)
	{
		if (Token.IsNumeric() || IsOperator(Token))
		{
			continue;
		}
		if (Token == TEXT("("))
		{
			++BracketCount;
		}
		else if (Token == TEXT(")"))
		{
			--BracketCount;
			if (BracketCount < 0)
			{
				UE_LOG(LogRsAbility, Error, TEXT("Invalid Script: Unmatched bracket"));
				return false;
			}
		}
		else if (!Token.EndsWith(TEXT(".Source")) && !Token.EndsWith(TEXT(".Target")))
		{
			UE_LOG(LogRsAbility, Error, TEXT("Invalid Script: [%s] Coefficient must have '.Source' or '.Target'"), *Token);
			return false;
		}
	}

	if (BracketCount != 0)
	{
		UE_LOG(LogRsAbility, Error, TEXT("Invalid Script: Unmatched bracket."));
		return false;
	}
	
	return true;
}

TArray<FString> FRsParser::ToPostfix(const TArray<FString>& Tokens) const
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

float FRsParser::GetCapturedAttributeMagnitude(const FString& Token, const FGameplayEffectSpec& Spec) const
{
	FGameplayEffectAttributeCaptureDefinition Def;
	Def.AttributeToCapture = URsAttributeSetBase::TagToAttribute(FGameplayTag::RequestGameplayTag(FName(Token)));
	if (Token.EndsWith(TEXT(".Source")))
	{
		Def.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
		Def.bSnapshot = true;
	}
	else if (Token.EndsWith(TEXT(".Target")))
	{
		Def.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
		Def.bSnapshot = false;
	}
	else
	{
		return 0.f;
	}
   
	const FGameplayEffectAttributeCaptureSpec* CaptureSpec = Spec.CapturedRelevantAttributes.FindCaptureSpecByDefinition(Def, true);
	if (CaptureSpec == nullptr)
	{
		UE_LOG(LogRsAbility, Error, TEXT("GetCapturedAttributeMagnitude unable to get capture spec."));
		return 0.f;
	}
	
	UE_LOG(LogRsAbility, Warning, TEXT("[CapDbg] Def: Source=%d Snapshot=%d Attr=%s"), (int32)Def.AttributeSource, (int32)Def.bSnapshot, *Def.AttributeToCapture.GetName());
	
	float Magnitude;
	if (CaptureSpec->AttemptCalculateAttributeMagnitude(FAggregatorEvaluateParameters(), Magnitude) == false)
	{
		UE_LOG(LogRsAbility, Error, TEXT("GetCapturedAttributeMagnitude unable to calculate attribute magnitude."));
		return 0.f;
	}
	UE_LOG(LogRsAbility, Warning, TEXT("[CapDbg] Magnitude: %f"), Magnitude);

	return Magnitude;
}

FString FRsParser::CalculateOperation(const FString& Value1, const FString& Value2, const FString& Operator) const
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

int32 FRsParser::GetPrecedence(const FString& Operator) const
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

bool FRsParser::IsOperator(const FString& Operator) const
{
	if (Operator == TEXT("+") || Operator == TEXT("-") || Operator == TEXT("*") || Operator == TEXT("/"))
	{
		return true;
	}
	return false;
}

bool FRsParser::IsOperator(const TCHAR& Operator) const
{
	if (Operator == '+' || Operator == '-' || Operator == '*' || Operator == '/')
	{
		return true;
	}
	return false;
}