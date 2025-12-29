// Copyright 2025 Team BH.


#include "RsCoefficientScriptExecution.h"

#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsAttributeSetBase.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Attributes/RsSpeedSet.h"
#include "Rs/AbilitySystem/Calculation/RsParser.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

URsCoefficientScriptExecution::URsCoefficientScriptExecution()
{
	RS_CAPTURE_ATTRIBUTE(URsAttackSet::GetAttackAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	RS_CAPTURE_ATTRIBUTE(URsSpeedSet::GetActionSpeedAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetBarrierAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	RS_CAPTURE_ATTRIBUTE(URsDefenseSet::GetDefenseAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetCurrentHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetMaxHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	RS_CAPTURE_ATTRIBUTE(URsAttackSet::GetImpactAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	
	RS_CAPTURE_ATTRIBUTE(URsAttackSet::GetAttackAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	RS_CAPTURE_ATTRIBUTE(URsSpeedSet::GetActionSpeedAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetBarrierAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	RS_CAPTURE_ATTRIBUTE(URsDefenseSet::GetDefenseAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetCurrentHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetMaxHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	RS_CAPTURE_ATTRIBUTE(URsAttackSet::GetImpactAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
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

void URsCoefficientScriptExecution::ApplyParseResult(const FRsEffectTableRow* Row, FName TableKey, const FGameplayAttribute& TargetAttribute, const FGameplayEffectSpec& Spec, FGameplayEffectCustomExecutionOutput& Output) const
{
	const FString Script = Row->FindValue<FString>(TableKey, false);
	const float Modifying = FRsParser::CoefficientScriptToFloat(Script, Spec);
	if (Modifying != 0.f)
	{
		Output.AddOutputModifier(FGameplayModifierEvaluatedData(TargetAttribute, EGameplayModOp::Additive, Modifying));
	}
}
