// Copyright 2025 Team BH.


#include "RsCoefficientScriptExecution.h"

#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"
#include "Rs/AbilitySystem/Calculation/RsCoefficientScriptParser.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

URsCoefficientScriptExecution::URsCoefficientScriptExecution()
{
	// TODO: CoefficientMap is not filled when construction!!!
	for (const auto [Tag, Coefficient] : URsAttributeSetBase::GetCoefficientMap())
	{
		FString CoeffString = Tag.ToString();
		if (CoeffString.EndsWith(TEXT(".Source")))
		{
			CaptureAttribute(Tag, Coefficient, EGameplayEffectAttributeCaptureSource::Source, true);
		}
		else if (CoeffString.EndsWith(TEXT(".Target")))
		{
			CaptureAttribute(Tag, Coefficient, EGameplayEffectAttributeCaptureSource::Target, false);
		}
	}
}

float URsCoefficientScriptExecution::FindAttributeMagnitude(FGameplayTag Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters) const
{
	float OutMagnitude = 0.f;
	if (CapturedAttributeDefinitions.Contains(Key))
	{
		const FGameplayEffectAttributeCaptureSpec* CaptureSpec = Spec.CapturedRelevantAttributes.FindCaptureSpecByDefinition(CapturedAttributeDefinitions[Key], true);
		CaptureSpec->AttemptCalculateAttributeMagnitude(EvaluationParameters, OutMagnitude);
	}
	else
	{
		UE_LOG(RsAbilityLog, Warning, TEXT("Cannot find [%s] attribute"), *Key.ToString());
	}
	return OutMagnitude;
}

void URsCoefficientScriptExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!TargetASC)
	{
		return;
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	if (const FRsEffectTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsEffectTableRow>(Spec))
	{
		for (const auto [Tag, Attribute] : URsAttributeSetBase::GetStatMap())
		{
			ApplyParseResult(Row, Tag.GetTagName(), Attribute, Spec, OutExecutionOutput);
		}
	}
}

void URsCoefficientScriptExecution::CaptureAttribute(FGameplayTag Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot)
{
	FGameplayEffectAttributeCaptureDefinition Definition;
	Definition.AttributeToCapture = Attribute;
	Definition.AttributeSource = SourceOrTarget;
	Definition.bSnapshot = bSnapShot;
	RelevantAttributesToCapture.Add(Definition);
	CapturedAttributeDefinitions.Add(Key, Definition);
}

void URsCoefficientScriptExecution::ApplyParseResult(const FRsEffectTableRow* Row, FName TableKey, const FGameplayAttribute& TargetAttribute, const FGameplayEffectSpec& Spec, FGameplayEffectCustomExecutionOutput& Output) const
{
	const FString Script = Row->FindValue<FString>(TableKey, false);
	const float ModifyingMagnitude = FRsCoefficientScriptParser::GetParseResult(Script, Spec, this);
	if (ModifyingMagnitude != 0.f)
	{
		Output.AddOutputModifier(FGameplayModifierEvaluatedData(TargetAttribute, EGameplayModOp::Additive, ModifyingMagnitude));
	}
}
