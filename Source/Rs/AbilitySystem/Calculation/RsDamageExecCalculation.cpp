// Copyright 2024 Team BH.


#include "RsDamageExecCalculation.h"

#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	RsDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, Attack, Source, false);
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
	
	// Damage Calculation
	float FinalDamage = FMath::Max((Attack * DamageCoefficient) - Defense, 0.f);

	if (FinalDamage <= 0.f || Attack < 0.f)
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
