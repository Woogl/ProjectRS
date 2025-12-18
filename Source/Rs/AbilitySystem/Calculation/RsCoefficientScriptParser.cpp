// Copyright 2025 Team BH.


#include "RsCoefficientScriptParser.h"

#include "GameplayTagContainer.h"
#include "Execution/RsCoefficientScriptExecution.h"
#include "ModifierMagnitude/RsCoefficientCalculationBase.h"
#include "Rs/RsLogChannels.h"

float FRsCoefficientScriptParser::GetParseResult(const FString& Script, const FGameplayEffectSpec& Spec, const URsCoefficientCalculationBase* Calc)
{
	if (Script.IsNumeric())
	{
		return FCString::Atof(*Script);
	}
	
	const FRsCoefficientScriptParser Parser = Get();
	
	const TArray<FString> Tokens = Parser.Tokenize(Script);

	if (!Parser.IsValid(Tokens))
	{
		UE_LOG(RsAbilityLog, Error, TEXT("Invalid Magnitude Script [%s] detected in [%s]."), *Script, *Spec.ToSimpleString());
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

	float Result = FCString::Atof(*Stack.Pop());
	return Result;
}

float FRsCoefficientScriptParser::GetParseResult(const FString& Script, const FGameplayEffectSpec& Spec, const URsCoefficientScriptExecution* Exec)
{
	if (Script.IsEmpty())
	{
		return 0.f;
	}
	
	if (Script.IsNumeric())
	{
		return FCString::Atof(*Script);
	}
	
	const FRsCoefficientScriptParser Parser = Get();
	
	const TArray<FString> Tokens = Parser.Tokenize(Script);

	if (!Parser.IsValid(Tokens))
	{
		UE_LOG(RsAbilityLog, Error, TEXT("Invalid Magnitude Script [%s] detected in [%s]."), *Script, *Spec.ToSimpleString());
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
				float AttributeValue = Exec->FindAttributeMagnitude(FGameplayTag::RequestGameplayTag(FName(Token)),Spec, EvaluationParameters);
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

const FRsCoefficientScriptParser& FRsCoefficientScriptParser::Get()
{
	static FRsCoefficientScriptParser Parser;
	return Parser;
}

TArray<FString> FRsCoefficientScriptParser::Tokenize(const FString& Script) const
{
	TArray<FString> Tokens;
	FString TempToken;

	for (TCHAR C : Script)
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

TArray<FString> FRsCoefficientScriptParser::ToPostfix(const TArray<FString>& Tokens) const
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

FString FRsCoefficientScriptParser::CalculateOperation(const FString& Value1, const FString& Value2, const FString& Operator) const
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


bool FRsCoefficientScriptParser::IsValid(const TArray<FString>& Tokens) const
{
	if(Tokens.IsEmpty())
	{
		UE_LOG(RsAbilityLog, Error, TEXT("Invalid Script detected."));
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
		UE_LOG(RsAbilityLog, Error, TEXT("Invalid Bracket Count detected."));
		return false;
	}
	
	return true;
}

int32 FRsCoefficientScriptParser::GetPrecedence(const FString& Operator) const
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

bool FRsCoefficientScriptParser::IsOperator(const FString& Operator) const
{
	if (Operator == TEXT("+") || Operator == TEXT("-") || Operator == TEXT("*") || Operator == TEXT("/"))
	{
		return true;
	}
	return false;
}

bool FRsCoefficientScriptParser::IsOperator(const TCHAR& Operator) const
{
	if (Operator == '+' || Operator == '-' || Operator == '*' || Operator == '/')
	{
		return true;
	}
	return false;
}