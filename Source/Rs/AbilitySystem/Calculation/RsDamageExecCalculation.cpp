// Copyright 2024 Team BH.


#include "RsDamageExecCalculation.h"

#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"
#include "Rs/System/RsGameSetting.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDmgBonus);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthDamage);

	RsDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, Attack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, CriticalRate, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, CriticalDmgBonus, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsDefenseSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, CurrentHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, HealthDamage, Target, false);
	}

	static const RsDamageStatics& Get()
	{
		static RsDamageStatics DamageStatics;
		return DamageStatics;
	}
};

URsDamageExecCalculation::URsDamageExecCalculation()
{
	const RsDamageStatics* DamageStatics = &RsDamageStatics::Get();
	
	RelevantAttributesToCapture.Add(DamageStatics->AttackDef);
	RelevantAttributesToCapture.Add(DamageStatics->CriticalRateDef);
	RelevantAttributesToCapture.Add(DamageStatics->CriticalDmgBonusDef);
	RelevantAttributesToCapture.Add(DamageStatics->DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics->CurrentHealthDef);
}

void URsDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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

	// During perfect dodge, ignore damage effect
	FGameplayTagContainer PerfectDodgeTags = FGameplayTag::RequestGameplayTag(TEXT("Ability.Dodge.Perfect")).GetSingleTagContainer();
	FGameplayEffectSpec ActivePerfectDodgeEffect = URsAbilitySystemLibrary::FindActiveGameplayEffectSpec(TargetASC, PerfectDodgeTags);
	if (ActivePerfectDodgeEffect.Def)
	{
		return;
	}

	const RsDamageStatics* DamageStatics = &RsDamageStatics::Get();

	// Set in RsGameplayAbility_Attack
	float DamageCoefficient = FMath::Max<float>(Spec.GetSetByCallerMagnitude(URsGameSetting::Get()->HealthDamageCoefficientTag, true, 1.f), 0.f);

	float Attack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->AttackDef, EvaluationParameters, Attack);
	// Attack shouldn't be minus value
	Attack = FMath::Max(Attack, 0.f);

	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->DefenseDef, EvaluationParameters, Defense);
	
	float CriticalRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->CriticalRateDef, EvaluationParameters, CriticalRate);
	// Critical Rate should be 0 ~ 100 range.
	CriticalRate = FMath::Clamp(CriticalRate, 0.f, 100.f);
	
	float CriticalDmgBonus = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->CriticalDmgBonusDef, EvaluationParameters, CriticalDmgBonus);

	// Check critical hit
	float RandomValue = FMath::RandRange(0.f, 100.f);
	bool bCriticalHit = CriticalRate >= RandomValue;
	if (bCriticalHit)
	{
		FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();
		FRsGameplayEffectContext* ContextHandle = static_cast<FRsGameplayEffectContext*>(MutableSpec->GetContext().Get());
		ContextHandle->bIsCriticalHit = true;
	}
	
	// Damage calculation start
	float FinalHealthDamage = (Attack * DamageCoefficient);
	// Critical calc
	FinalHealthDamage *= (bCriticalHit ? (CriticalDmgBonus * 0.01f) : 1.f);
	// Defense rate calc
	FinalHealthDamage *= (190.f / (Defense + 190.f));
	
	if (FinalHealthDamage <= 0.f || Attack <= 0.f)
	{
		return;
	}

	// Stun has 160 % damage bonus
	if (EvaluationParameters.TargetTags->HasTagExact(URsGameSetting::Get()->StunAbilityTag))
	{
		FinalHealthDamage *= 1.6f;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->HealthDamageProperty, EGameplayModOp::Additive, FinalHealthDamage));
}
