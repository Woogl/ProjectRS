// Copyright 2024 Team BH.


#include "RsStaggerDamageExecution.h"

#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"
#include "Rs/System/RsGameSetting.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsStaggerDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaggerDamage);

	RsStaggerDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsStaggerSet, BaseDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsStaggerSet, StaggerDamage, Target, false);
	}

	static const RsStaggerDamageStatics& Get()
	{
		static RsStaggerDamageStatics StaggerStatics;
		return StaggerStatics;
	}
};

URsStaggerDamageExecution::URsStaggerDamageExecution()
{
	const RsStaggerDamageStatics* DamageStatics = &RsStaggerDamageStatics::Get();

	RelevantAttributesToCapture.Add(DamageStatics->BaseDamageDef);
}

void URsStaggerDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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
		// Don't trigger gameplay cue.
		OutExecutionOutput.MarkGameplayCuesHandledManually();
		return;
	}

	const RsStaggerDamageStatics* DamageStatics = &RsStaggerDamageStatics::Get();

	// Set in RsCoefficientCalculation
	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->BaseDamageDef, EvaluationParameters, BaseDamage);

	if (BaseDamage <= 0.f)
	{
		return;
	}
	
	// Stagger calculation start
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

	OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->StaggerDamageProperty, EGameplayModOp::Additive, FinalDamage));
}
