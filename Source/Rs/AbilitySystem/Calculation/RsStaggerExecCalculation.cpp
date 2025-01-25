// Copyright 2024 Team BH.


#include "RsStaggerExecCalculation.h"

#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"
#include "Rs/System/RsGameplayTags.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsStaggerStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Impact);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentStagger);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	RsStaggerStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, Impact, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsStaggerSet, CurrentStagger, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsStaggerSet, Damage, Target, false);
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

	// Set in RsGameplayAbility_Attack
	float StaggerCoefficient = FMath::Max<float>(Spec.GetSetByCallerMagnitude(RsGameplayTags::TAG_COEFFICIENT_DAMAGE_STAGGER, true, 1.f), 0.f);

	float Impact = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->ImpactDef, EvaluationParameters, Impact);
	// Impact shouldn't be minus value
	Impact = FMath::Max(Impact, 0.f);
	
	// Stagger Calculation
	float FinalStaggerGain = Impact * StaggerCoefficient;

	if (FinalStaggerGain <= 0.f || Impact < 0.f)
	{
		return;
	}

	// Stun has 160 % damage bonus
	if (EvaluationParameters.TargetTags->HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Ability.Stun"))))
	{
		FinalStaggerGain *= 1.6f;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->DamageProperty, EGameplayModOp::Additive, FinalStaggerGain));
}
