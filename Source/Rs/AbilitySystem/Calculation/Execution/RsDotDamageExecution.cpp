// Copyright 2024 Team BH.


#include "RsDotDamageExecution.h"

#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsDotDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthDamage);

	RsDotDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, BaseDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsDefenseSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, HealthDamage, Target, false);
	}

	static const RsDotDamageStatics& Get()
	{
		static RsDotDamageStatics DamageStatics;
		return DamageStatics;
	}
};

URsDotDamageExecution::URsDotDamageExecution()
{
	const RsDotDamageStatics* DamageStatics = &RsDotDamageStatics::Get();

	RelevantAttributesToCapture.Add(DamageStatics->BaseDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics->DefenseDef);
}

void URsDotDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (TargetASC == nullptr)
	{
		return;
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	float Duration = Spec.GetDuration();
	float Period = Spec.GetPeriod();

	// we don't consider about invalid dot damage
	if (Duration <= 0.f || Period <= 0.f)
	{
		return;
	}

	int32 Tick = Duration / Period;
	FAggregatorEvaluateParameters EvaluationParameters{};
	const RsDotDamageStatics* DamageStatics = &RsDotDamageStatics::Get();

	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->BaseDamageDef, EvaluationParameters, BaseDamage);
	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->DefenseDef, EvaluationParameters, Defense);

	float TickDamage = BaseDamage / Tick;
	TickDamage *= (190.f / (Defense + 190.f));
	
	if (TickDamage > 0)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->HealthDamageProperty, EGameplayModOp::Additive, TickDamage));
	}
}
