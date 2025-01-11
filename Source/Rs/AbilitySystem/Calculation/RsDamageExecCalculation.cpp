// Copyright 2024 Team BH.


#include "RsDamageExecCalculation.h"

#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDmgBonus);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	RsDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, Attack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, CriticalRate, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, CriticalDmgBonus, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsDefenseSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, CurrentHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, Damage, Target, false);
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

	const RsDamageStatics* DamageStatics = &RsDamageStatics::Get();

	// Set in RsGameplayAbility_Attack
	float DamageCoefficient = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FName("DamageCoefficient"), true, 1.f), 0.f);

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
	float FinalDamage = (Attack * DamageCoefficient);
	// Critical calc
	FinalDamage *= (bCriticalHit ? (CriticalDmgBonus * 0.01f) : 1.f);
	// Defense rate calc
	FinalDamage *= (190.f / (Defense + 190.f));
	
	if (FinalDamage <= 0.f || Attack <= 0.f)
	{
		return;
	}

	// Stun has 160 % damage bonus
	if (EvaluationParameters.TargetTags->HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Ability.Stun"))))
	{
		FinalDamage *= 1.6f;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->DamageProperty, EGameplayModOp::Additive, FinalDamage));
}
