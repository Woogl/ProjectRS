// Copyright 2024 Team BH.


#include "RsStaggerDamageExecution.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsStaggerDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaggerDamageBonus);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Resistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FinalDamage);

	RsStaggerDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsStaggerSet, BaseDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, StaggerDamageBonus, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsDefenseSet, Resistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsStaggerSet, FinalDamage, Target, false);
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
	if (!TargetASC)
	{
		return;
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluationParameters{};
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	if (EvaluationParameters.TargetTags->HasTag(RsGameplayTags::ABILITY_DEATH))
	{
		// Don't show hit VFX.
		OutExecutionOutput.MarkGameplayCuesHandledManually();
		return;
	}

	const RsStaggerDamageStatics* DamageStatics = &RsStaggerDamageStatics::Get();

	// Set in RsCoefficientCalculation
	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->BaseDamageDef, EvaluationParameters, BaseDamage);
	
	// Stagger calculation start
	float FinalDamage = BaseDamage;
	
	// Damage bonus calc
	float StaggerDamageBonus = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->StaggerDamageBonusDef, EvaluationParameters, StaggerDamageBonus);
	FinalDamage *= (1 + StaggerDamageBonus * 0.01f);
	
	// Resistance calc
	float Resistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->ResistanceDef, EvaluationParameters, Resistance);
	FinalDamage *= (1 - Resistance * 0.01f);

	OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->FinalDamageProperty, EGameplayModOp::Override, FinalDamage));
}
