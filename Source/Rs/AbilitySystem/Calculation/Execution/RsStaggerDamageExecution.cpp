// Copyright 2024 Team BH.


#include "RsStaggerDamageExecution.h"

#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"
#include "Rs/System/RsGameSettingDataAsset.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsStaggerDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FinalDamage);

	RsStaggerDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsStaggerSet, BaseDamage, Target, false);
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
	if (EvaluationParameters.TargetTags->HasTag(URsGameSettingDataAsset::Get().DeathAbilityTag))
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

	OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->FinalDamageProperty, EGameplayModOp::Override, FinalDamage));
}
