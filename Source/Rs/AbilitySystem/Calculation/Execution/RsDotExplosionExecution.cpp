// Copyright 2024 Team BH.


#include "RsDotExplosionExecution.h"
#include "AbilitySystemComponent.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RsDotExplosionDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthDamage);

	RsDotExplosionDamageStatics()
	{
		// Capture optional attribute set
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsDefenseSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URsHealthSet, HealthDamage, Target, false);
	}

	static const RsDotExplosionDamageStatics& Get()
	{
		static RsDotExplosionDamageStatics DamageStatics;
		return DamageStatics;
	}
};

void URsDotExplosionExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (TargetASC == nullptr)
	{
		return;
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	FGameplayTagContainer DotDamageTags;
	Spec.GetAllAssetTags(DotDamageTags);

	FGameplayEffectQuery DotDamageQuery = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(DotDamageTags);
	TArray<FActiveGameplayEffectHandle> ActiveDotEffects = TargetASC->GetActiveEffects(DotDamageQuery);

	int32 DotStack = 0;
	float TotalDamage = 0;

	for (FActiveGameplayEffectHandle DotEffectHandle : ActiveDotEffects)
	{
		const FActiveGameplayEffect* DotEffect = TargetASC->GetActiveGameplayEffect(DotEffectHandle);
		float DotDuration = DotEffect->GetDuration();
		float DotPeriod = DotEffect->GetPeriod();

		// we don't consider about invalid dot damage
		if (DotDuration <= 0.f || DotPeriod <= 0.f)
		{
			break;
		}
		int32 TotalTick = DotDuration / DotPeriod;

		float DotTotalDamage = TargetASC->GetGameplayEffectMagnitude(DotEffectHandle, URsHealthSet::GetBaseDamageAttribute());
		float DotTickDamage = DotTotalDamage / TotalTick;
		
		float LastTime = DotEffect->GetEndTime() - TargetASC->GetWorld()->GetTimeSeconds();
		float LastTick = LastTime / DotEffect->GetPeriod();

		TotalDamage += DotTickDamage * LastTick;

		TargetASC->RemoveActiveGameplayEffect(DotEffectHandle);
		DotStack++;
	}
	
	FAggregatorEvaluateParameters EvaluationParameters{};
	const RsDotExplosionDamageStatics* DamageStatics = &RsDotExplosionDamageStatics::Get();
	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->DefenseDef, EvaluationParameters, Defense);
	
	TotalDamage *= (1 + DotStack * Spec.GetSetByCallerMagnitude(TEXT("DamageMultiplierPerDotStacks")));
	TotalDamage *= (190.f / (Defense + 190.f));

	if (TotalDamage > 0)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(URsHealthSet::GetHealthDamageAttribute(), EGameplayModOp::Additive, TotalDamage));
	}
}
