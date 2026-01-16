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

float URsCoefficientScriptCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FRsEffectTableRow* Row = URsAbilitySystemGlobals::GetEffectTableRow<FRsEffectTableRow>(Spec.GetContext());
	if (!Row)
	{
		UE_LOG(LogRsAbility, Warning, TEXT("No effect table row in Spec: %s"), *Spec.ToSimpleString());
		return 0.f;
	}
	
	if (TableKey.IsNone())
	{
		UE_LOG(LogRsAbility, Warning, TEXT("Table key is None: %s"), *Spec.ToSimpleString());
		return 0.f;
	}

	const FString Script = Row->FindValue<FString>(TableKey, false);
	return FRsParser::CoefficientScriptToFloat(Script, Spec);
}
