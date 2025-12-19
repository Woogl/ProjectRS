// Copyright 2025 Team BH.


#include "RsCoefficientScriptExecution.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Attributes/RsSpeedSet.h"
#include "Rs/AbilitySystem/Calculation/RsCoefficientScriptParser.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

URsCoefficientScriptExecution::URsCoefficientScriptExecution()
{
	CaptureAttribute(RsGameplayTags::COEFFICIENT_ATK_SOURCE, URsAttackSet::GetAttackAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_ATS_SOURCE, URsSpeedSet::GetActionSpeedAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_BP_SOURCE, URsHealthSet::GetBarrierAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_DEF_SOURCE, URsDefenseSet::GetDefenseAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_HPcur_SOURCE, URsHealthSet::GetCurrentHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_HPmax_SOURCE, URsHealthSet::GetMaxHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_IMP_SOURCE, URsAttackSet::GetImpactAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	
	CaptureAttribute(RsGameplayTags::COEFFICIENT_ATK_TARGET, URsAttackSet::GetAttackAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_ATS_TARGET, URsSpeedSet::GetActionSpeedAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_BP_TARGET, URsHealthSet::GetBarrierAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_DEF_TARGET, URsDefenseSet::GetDefenseAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_HPcur_TARGET, URsHealthSet::GetCurrentHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_HPmax_TARGET, URsHealthSet::GetMaxHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	CaptureAttribute(RsGameplayTags::COEFFICIENT_IMP_TARGET, URsAttackSet::GetImpactAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
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
