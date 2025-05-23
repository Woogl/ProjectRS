// Copyright 2024 Team BH.


#include "RsDotBurstDamageExecution.h"
#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Effect/RsEffectDefinition.h"
#include "Rs/System/RsGameSetting.h"

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

void URsDotBurstDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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
	
	double WorldTime = TargetASC->GetWorld()->GetTimeSeconds();

	int32 DotStack = 0;
	float TotalDamage = 0;
	for (FActiveGameplayEffectHandle DotEffectHandle : ActiveDotEffects)
	{
		
		const FActiveGameplayEffect* DotEffect = TargetASC->GetActiveGameplayEffect(DotEffectHandle);
		if (DotEffect == nullptr)
		{
			continue;
		}
		
		float TimeRemaining = DotEffect->GetTimeRemaining(WorldTime);
		float Period = DotEffect->GetPeriod();
		int32 RemainingTicks = FMath::FloorToInt(TimeRemaining / Period);
		if (TimeRemaining <= 0.f || Period <= 0.f || RemainingTicks <= 0)
		{
			continue;
		}

		float TotalDotDamage = TargetASC->GetGameplayEffectMagnitude(DotEffectHandle, URsHealthSet::GetBaseDamageAttribute());
		// Infinite DoT damage's duration is zero.
		float TickDamage = TotalDotDamage / FMath::Max(1, static_cast<int32>(DotEffect->GetDuration() / Period));
		float RemainingDamage = TickDamage * RemainingTicks;
		TotalDamage += RemainingDamage;

		TargetASC->RemoveActiveGameplayEffect(DotEffectHandle);
		DotStack++;
	}
	
	if (TotalDamage <= 0.f)
	{
		return;
	}
	
	FAggregatorEvaluateParameters EvaluationParameters{};
	const RsDotExplosionDamageStatics* DamageStatics = &RsDotExplosionDamageStatics::Get();
	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->DefenseDef, EvaluationParameters, Defense);

	float DefenseConstant = URsGameSetting::Get()->DefenseConstant;
	const FName PropertyName = GET_MEMBER_NAME_CHECKED(URsEffectDefinition_DotBurstDamage, DamageMultiplierPerDotStacks);
	TotalDamage *= (1 + DotStack * Spec.GetSetByCallerMagnitude(PropertyName));
	TotalDamage *= (DefenseConstant / (Defense + DefenseConstant));

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(URsHealthSet::GetHealthDamageAttribute(), EGameplayModOp::Additive, TotalDamage));
}
