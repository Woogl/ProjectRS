// Copyright 2024 Team BH.


#include "RsHealthDamageExecution.h"

#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemSettings.h"
#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffectContext.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsHealthDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FinalDamage);

	RsHealthDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, BaseDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, CriticalRate, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsAttackSet, CriticalDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsDefenseSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, FinalDamage, Target, false);
	}

	static const RsHealthDamageStatics& Get()
	{
		static RsHealthDamageStatics DamageStatics;
		return DamageStatics;
	}
};

URsHealthDamageExecution::URsHealthDamageExecution()
{
	const RsHealthDamageStatics* DamageStatics = &RsHealthDamageStatics::Get();

	RelevantAttributesToCapture.Add(DamageStatics->BaseDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics->CriticalRateDef);
	RelevantAttributesToCapture.Add(DamageStatics->CriticalDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics->DefenseDef);
}

void URsHealthDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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
		// Don't show damage floater.
		OutExecutionOutput.MarkGameplayCuesHandledManually();
		return;
	}

	const RsHealthDamageStatics* DamageStatics = &RsHealthDamageStatics::Get();

	// Set in RsCoefficientCalculation
	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->BaseDamageDef, EvaluationParameters, BaseDamage);

	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->DefenseDef, EvaluationParameters, Defense);
	
	float CriticalRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->CriticalRateDef, EvaluationParameters, CriticalRate);
	// Critical Rate should be 0 ~ 100 range.
	CriticalRate = FMath::Clamp(CriticalRate, 0.f, 100.f);
	
	float CriticalDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->CriticalDamageDef, EvaluationParameters, CriticalDamage);
	
	// Damage calculation start
	float FinalDamage = BaseDamage;

	// Check critical hit
	float RandomValue = FMath::RandRange(0.f, 100.f);
	bool bCriticalHit = CriticalRate > RandomValue;
	if (bCriticalHit)
	{
		FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();
		FRsGameplayEffectContext* ContextHandle = static_cast<FRsGameplayEffectContext*>(MutableSpec->GetContext().Get());
		ContextHandle->bIsCriticalHit = true;
	}
	
	// Critical damage calc
	FinalDamage *= bCriticalHit ? (1 + CriticalDamage * 0.01f) : 1.f;
	
	// Damage reduction rate calc
	float DefenseConstant = URsAbilitySystemSettings::Get().DamageReductionConstant;
	FinalDamage *= (DefenseConstant / (Defense + DefenseConstant));

	// Groggy has 160 % damage bonus
	if (EvaluationParameters.TargetTags->HasTag(RsGameplayTags::ABILITY_GROGGY))
	{
		FinalDamage *= URsAbilitySystemSettings::Get().GroggyDamageMultiplier;
	}

	OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->FinalDamageProperty, EGameplayModOp::Override, FinalDamage));
	
	// Feedback to source
	FGameplayEventData Payload;
	Payload.EventTag = RsGameplayTags::EFFECT_DAMAGE_FEEDBACK;
	Payload.Instigator = ExecutionParams.GetSourceAbilitySystemComponent()->GetOwner();
	Payload.Target = ExecutionParams.GetTargetAbilitySystemComponent()->GetOwner();
	Payload.EventMagnitude = FinalDamage;
	ExecutionParams.GetSourceAbilitySystemComponent()->HandleGameplayEvent(RsGameplayTags::EFFECT_DAMAGE_FEEDBACK, &Payload);
}
