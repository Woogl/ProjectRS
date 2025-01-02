// Copyright 2024 Team BH.


#include "RsStaggerExecCalculation.h"

#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsStaggerStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Impact);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Will);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentStagger);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaggerGain);

	RsStaggerStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, Impact, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsDefenseSet, Will, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsStaggerSet, CurrentStagger, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsStaggerSet, StaggerGain, Target, false);
	}

	static const RsStaggerStatics& Get()
	{
		static RsStaggerStatics StaggerStatics;
		return StaggerStatics;
	}
};

URsStaggerExecCalculation::URsStaggerExecCalculation()
{
	const RsStaggerStatics* DamageStatics = &RsStaggerStatics::Get();
	
	RelevantAttributesToCapture.Add(DamageStatics->ImpactDef);
	RelevantAttributesToCapture.Add(DamageStatics->WillDef);
	RelevantAttributesToCapture.Add(DamageStatics->CurrentStaggerDef);
}

void URsStaggerExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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

	float Impact = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->ImpactDef, EvaluationParameters, Impact);
	Impact = FMath::Max(Impact, 0.f);

	float Will = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->WillDef, EvaluationParameters, Will);
	// Will can be minus value
	
	// Stagger Calculation
	float FinalStaggerGain = FMath::Max(Impact - Will, 0.f);

	if (FinalStaggerGain <= 0.f || Impact < 0.f)
	{
		return;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->StaggerGainProperty, EGameplayModOp::Additive, FinalStaggerGain));
}
