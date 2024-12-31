// Copyright 2024 Team BH.


#include "RsDamageExecCalculation.h"

#include "Rs/AbilitySystem/Attributes/AttackSet.h"
#include "Rs/AbilitySystem/Attributes/DefenseSet.h"
#include "Rs/AbilitySystem/Attributes/HealthSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);

	RsDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttackSet, Attack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDefenseSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthSet, CurrentHealth, Target, false);
	}

	static const RsDamageStatics& Get()
	{
		static RsDamageStatics DmgStatics;
		return DmgStatics;
	}
};

URsDamageExecCalculation::URsDamageExecCalculation()
{
	const RsDamageStatics* DamageStatics = &RsDamageStatics::Get();
	
	RelevantAttributesToCapture.Add(DamageStatics->AttackDef);
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

	float Attack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->AttackDef, EvaluationParameters, Attack);
	Attack = FMath::Max(Attack, 0.f);

	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->DefenseDef, EvaluationParameters, Defense);
	// Defense can be minus value
	
	// Damage Calculation
	float FinalDamage = FMath::Max(Attack - Defense, 0.f);

	if (FinalDamage <= 0.f || Attack < 0.f)
	{
		return;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->CurrentHealthProperty, EGameplayModOp::Additive, -FinalDamage));
}
