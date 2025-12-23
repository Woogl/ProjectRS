// Copyright 2024 Team BH.


#include "RsCoefficientScriptCalculation.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Attributes/RsSpeedSet.h"
#include "Rs/AbilitySystem/Calculation/RsParser.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"

URsCoefficientScriptCalculation::URsCoefficientScriptCalculation()
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

float URsCoefficientScriptCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FRsEffectTableRow* Row = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsEffectTableRow>(Spec);
	if (!Row)
	{
		UE_LOG(LogRsAbility, Warning, TEXT("No effect table row in Spec: %s"), *Spec.ToSimpleString());
		return 0.f;
	}

	for (const FGameplayModifierInfo& Modifier : Spec.Def->Modifiers)
	{
		if (Modifier.ModifierMagnitude.GetCustomMagnitudeCalculationClass() != GetClass())
		{
			continue;
		}
		const FGameplayTag ModifierStatTag = URsAttributeSetBase::AttributeToTag(Modifier.Attribute);
		if (!ModifierStatTag.IsValid())
		{
			continue;
		}
		const FString Script = Row->FindValue<FString>(ModifierStatTag.GetTagName(), false);
		if (Script.IsEmpty())
		{
			continue;
		}
		return FRsParser::CoefficientScriptToFloat(Script, Spec, this);
	}
	
	UE_LOG(LogRsAbility, Warning, TEXT("Cannot find coefficient script of [%s]"), *Spec.ToSimpleString());
	return 0.f;
}

float URsCoefficientScriptCalculation::FindAttributeMagnitude(FGameplayTag Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters) const
{
	float OutMagnitude = 0.f;
	if (CapturedAttributeDefinitions.Contains(Key))
	{
		GetCapturedAttributeMagnitude(CapturedAttributeDefinitions[Key], Spec, EvaluationParameters, OutMagnitude);
	}
	else
	{
		UE_LOG(LogRs, Warning, TEXT("Cannot find [%s] attribute"), *Key.ToString());
	}
	return OutMagnitude;
}

void URsCoefficientScriptCalculation::CaptureAttribute(FGameplayTag Key, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource SourceOrTarget, bool bSnapShot)
{
	FGameplayEffectAttributeCaptureDefinition Definition;
	Definition.AttributeToCapture = Attribute;
	Definition.AttributeSource = SourceOrTarget;
	Definition.bSnapshot = bSnapShot;
	RelevantAttributesToCapture.Add(Definition);
	CapturedAttributeDefinitions.Add(Key, Definition);
}
