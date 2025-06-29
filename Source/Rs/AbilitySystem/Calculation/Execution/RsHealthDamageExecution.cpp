// Copyright 2024 Team BH.


#include "RsHealthDamageExecution.h"

#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"
#include "Rs/System/RsGameSetting.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsHealthDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDmgBonus);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthDamage);

	RsHealthDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, BaseDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, CriticalRate, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, CriticalDmgBonus, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatics->CriticalDmgBonusDef);
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
	if (EvaluationParameters.TargetTags->HasTag(URsGameSetting::Get()->DeathAbilityTag))
	{
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
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->CriticalDmgBonusDef, EvaluationParameters, CriticalBonus);

	if (BaseDamage <= 0.f)
	{
		return;
	}
	
	// Damage calculation start
	float FinalDamage = BaseDamage;
	
	// Chceck DoT damage
	float Duration = Spec.GetDuration();
	float Period = Spec.GetPeriod();
	bool bDotDamage = Duration > 0.f && Period > 0.f;
	if (bDotDamage)
	{
		float Tick = Duration / Period;
		FinalDamage /= Tick;
	}

	// Check critical hit
	float RandomValue = FMath::RandRange(0.f, 100.f);
	bool bCriticalHit = CriticalRate >= RandomValue;
	// Disable critical when DoT damage
	if (bCriticalHit && !bDotDamage)
	{
		FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();
		FRsGameplayEffectContext* ContextHandle = static_cast<FRsGameplayEffectContext*>(MutableSpec->GetContext().Get());
		ContextHandle->bIsCriticalHit = true;
	}
	
	// Critical calc
	FinalDamage *= ((bCriticalHit && !bDotDamage) ? (1 + CriticalBonus * 0.01f) : 1.f);
	// Defense rate calc
	float DefenseConstant = URsGameSetting::Get()->DefenseConstant;
	FinalDamage *= (DefenseConstant / (Defense + DefenseConstant));

	// Groggy has 160 % damage bonus
	if (EvaluationParameters.TargetTags->HasTagExact(URsGameSetting::Get()->GroggyAbilityTag))
	{
		FinalDamage *= 1.6f;
	}

	OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->HealthDamageProperty, EGameplayModOp::Additive, FinalDamage));
}
