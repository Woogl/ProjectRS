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
	RS_CAPTURE_ATTRIBUTE(URsAttackSet::GetAttackAttribute(), Source, true);
	RS_CAPTURE_ATTRIBUTE(URsSpeedSet::GetActionSpeedAttribute(), Source, true);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetBarrierAttribute(), Source, true);
	RS_CAPTURE_ATTRIBUTE(URsDefenseSet::GetDefenseAttribute(), Source, true);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetCurrentHealthAttribute(), Source, true);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetMaxHealthAttribute(), Source, true);
	RS_CAPTURE_ATTRIBUTE(URsAttackSet::GetImpactAttribute(), Source, true);
	
	RS_CAPTURE_ATTRIBUTE(URsAttackSet::GetAttackAttribute(), Target, false);
	RS_CAPTURE_ATTRIBUTE(URsSpeedSet::GetActionSpeedAttribute(), Target, false);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetBarrierAttribute(), Target, false);
	RS_CAPTURE_ATTRIBUTE(URsDefenseSet::GetDefenseAttribute(), Target, false);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetCurrentHealthAttribute(), Target, false);
	RS_CAPTURE_ATTRIBUTE(URsHealthSet::GetMaxHealthAttribute(), Target, false);
	RS_CAPTURE_ATTRIBUTE(URsAttackSet::GetImpactAttribute(), Target, false);
}

void URsCoefficientScriptExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!TargetASC)
	{
		return;
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	if (const FRsEffectTableRow* Row = URsAbilitySystemGlobals::GetEffectTableRow<FRsEffectTableRow>(Spec.GetContext()))
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
