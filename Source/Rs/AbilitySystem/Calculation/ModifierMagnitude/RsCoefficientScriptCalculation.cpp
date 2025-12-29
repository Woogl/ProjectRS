// Copyright 2024 Team BH.


#include "RsCoefficientScriptCalculation.h"

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
		return FRsParser::CoefficientScriptToFloat(Script, Spec);
	}
	
	UE_LOG(LogRsAbility, Warning, TEXT("Cannot find coefficient script of [%s]"), *Spec.ToSimpleString());
	return 0.f;
}
