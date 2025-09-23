// Copyright 2024 Team BH.


#include "RsHealthDamageExecution.h"

#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"
#include "Rs/System/RsGameSettingDataAsset.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsHealthDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthDamage);

	RsHealthDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, BaseDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, CriticalRate, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, CriticalDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsDefenseSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, HealthDamage, Target, false);
	}

	static const RsHealthDamageStatics& Get()
	{
		static RsHealthDamageStatics DamageStatics;
		return DamageStatics;
	}
};

URsHealthDamageExecution::URsHealthDamageExecution()
{
	const RsHealthDamageStatics* DamageStatics = &RsHealthDamageStatics::Get();

	RelevantAttributesToCapture.Add(DamageStatics->BaseDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics->CriticalRateDef);
	RelevantAttributesToCapture.Add(DamageStatics->CriticalDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics->DefenseDef);
}

void URsHealthDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (TargetASC == nullptr)
	{
		return;
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluationParameters{};
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	if (EvaluationParameters.TargetTags->HasTag(URsGameSettingDataAsset::Get()->DeathAbilityTag))
	{
		// Don't show damage floater.
		OutExecutionOutput.MarkGameplayCuesHandledManually();
		return;
	}

	const RsHealthDamageStatics* DamageStatics = &RsHealthDamageStatics::Get();

	// Set in RsCoefficientCalculation
	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->BaseDamageDef, EvaluationParameters, BaseDamage);

	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->DefenseDef, EvaluationParameters, Defense);
	
	float CriticalRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->CriticalRateDef, EvaluationParameters, CriticalRate);
	// Critical Rate should be 0 ~ 100 range.
	CriticalRate = FMath::Clamp(CriticalRate, 0.f, 100.f);
	
	float CriticalBonus = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->CriticalDamageDef, EvaluationParameters, CriticalBonus);

	if (BaseDamage <= 0.f)
	{
		return;
	}
	
	// Damage calculation start
	float FinalDamage = BaseDamage;
	
	// Chceck DoT damage
	bool bIsDotDamage = Spec.Def && Spec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration;
	if (bIsDotDamage)
	{
		HandleDotDamage(ExecutionParams, Spec, FinalDamage);
		if (FinalDamage <= 0.f)
		{
			// Don't show damage floater.
			OutExecutionOutput.MarkGameplayCuesHandledManually();
		}
	}

	// Check critical hit
	float RandomValue = FMath::RandRange(0.f, 100.f);
	bool bCriticalHit = CriticalRate >= RandomValue;
	if (bCriticalHit && !bIsDotDamage)
	{
		FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();
		FRsGameplayEffectContext* ContextHandle = static_cast<FRsGameplayEffectContext*>(MutableSpec->GetContext().Get());
		ContextHandle->bIsCriticalHit = true;
	}
	
	// Critical calc
	FinalDamage *= ((bCriticalHit && !bIsDotDamage) ? (1 + CriticalBonus * 0.01f) : 1.f);
	// Defense rate calc
	float DefenseConstant = URsGameSettingDataAsset::Get()->DefenseConstant;
	FinalDamage *= (DefenseConstant / (Defense + DefenseConstant));

	// Groggy has 160 % damage bonus
	if (EvaluationParameters.TargetTags->HasTagExact(URsGameSettingDataAsset::Get()->GroggyAbilityTag))
	{
		FinalDamage *= URsGameSettingDataAsset::Get()->GroggyDamageMultiplier;
	}

	OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->HealthDamageProperty, EGameplayModOp::Override, FinalDamage));
}

void URsHealthDamageExecution::HandleDotDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, float& OutDamage) const
{
	float Duration = Spec.GetDuration();
	float Period = Spec.GetPeriod();
	if (Duration > 0.f && Period > 0.f)
	{
		float RemainingTime = Spec.GetSetByCallerMagnitude(DataName_RemainingTime, false, Duration);
		if (RemainingTime < Duration - KINDA_SMALL_NUMBER)
		{
			float Tick = Duration / Period;
			OutDamage /= Tick;
		}
		else
		{
			OutDamage = 0.f;
		}

		if (FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod())
		{
			FRsGameplayEffectContext* ContextHandle = static_cast<FRsGameplayEffectContext*>(MutableSpec->GetContext().Get());
			ContextHandle->bIsDotDamage = true;
			// Disable critical when DoT damage
			ContextHandle->bIsCriticalHit = false;
			// Update remaining time
			float NextRemainingTime = FMath::Max(RemainingTime - Period, 0.f);
			MutableSpec->SetSetByCallerMagnitude(DataName_RemainingTime, NextRemainingTime);
		}
	}
}