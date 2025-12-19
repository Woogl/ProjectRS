// Copyright 2024 Team BH.


#include "RsCoefficientCalculationBase.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Attributes/RsSpeedSet.h"

URsCoefficientCalculationBase::URsCoefficientCalculationBase()
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

float URsCoefficientCalculationBase::FindAttributeMagnitude(FGameplayTag Key, const FGameplayEffectSpec& Spec, const FAggregatorEvaluateParameters& EvaluationParameters) const
{
	float OutMagnitude = 0.f;
	if (CapturedAttributeDefinitions.Contains(Key))
	{
		GetCapturedAttributeMagnitude(CapturedAttributeDefinitions[Key], Spec, EvaluationParameters, OutMagnitude);
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("Cannot find [%s] attribute"), *Key.ToString());
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
