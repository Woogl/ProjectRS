// Copyright 2024 Team BH.


#include "RsStaggerDamageCalculation.h"

#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsStaggerStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaggerDamage);

	RsStaggerStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsStaggerSet, BaseDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsStaggerSet, StaggerDamage, Target, false);
	}

	static const RsStaggerStatics& Get()
	{
		static RsStaggerStatics StaggerStatics;
		return StaggerStatics;
	}
};

URsStaggerDamageCalculation::URsStaggerDamageCalculation()
{
	const RsStaggerStatics* DamageStatics = &RsStaggerStatics::Get();

	RelevantAttributesToCapture.Add(DamageStatics->BaseDamageDef);
}

void URsStaggerDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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

	const RsStaggerStatics* DamageStatics = &RsStaggerStatics::Get();

	// Set in RsCoefficientCalculation
	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->BaseDamageDef, EvaluationParameters, BaseDamage);
	
	// Stagger Calculation
	float FinalDamage = BaseDamage;

	if (FinalDamage <= 0.f)
	{
		return;
	}

	OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->StaggerDamageProperty, EGameplayModOp::Additive, FinalDamage));
}
