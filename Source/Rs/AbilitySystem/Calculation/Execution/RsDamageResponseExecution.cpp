// Copyright 2025 Team BH.


#include "RsDamageResponseExecution.h"

#include "AbilitySystemComponent.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"
#include "Rs/AbilitySystem/EffectComponent/RsDamageEffectComponent.h"

void URsDamageResponseExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!TargetASC || !SourceASC)
	{
		return;
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluationParameters{};
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	const URsDamageEffectComponent* DamageGEComp = Spec.Def->FindComponent<URsDamageEffectComponent>();
	if (!DamageGEComp)
	{
		return;
	}
	const FRsDamageEffectTableRow* DamageTableRow = DamageGEComp->GetDamageTableRow();

	// Check super armor
	int32 SuperArmorPierce = DamageTableRow ? DamageTableRow->SuperArmorPierce : DamageGEComp->SuperArmorPierce;
	float SuperArmor = URsAbilitySystemLibrary::GetNumericAttributeByTag(TargetASC, RsGameplayTags::STAT_SA);
	if (SuperArmorPierce >= SuperArmor)
	{
		// Trigger hit reaction
		FGameplayEventData Payload;
		Payload.EventTag = DamageTableRow ? DamageTableRow->HitReaction : DamageGEComp->HitReaction;
		Payload.Instigator = Spec.GetEffectContext().GetInstigator();
		Payload.Target = TargetASC->GetOwner();
		Payload.InstigatorTags = Spec.CapturedSourceTags.GetActorTags();
		Payload.TargetTags = Spec.CapturedTargetTags.GetActorTags();
		Payload.ContextHandle = Spec.GetEffectContext();
			
		TargetASC->HandleGameplayEvent(Payload.EventTag, &Payload);
	}

	// Trigger hit stop
	const float SourceHitStopTime = DamageTableRow ? DamageTableRow->SourceHitStopTime : DamageGEComp->SourceHitStopTime;
	if (SourceHitStopTime > 0.f && SourceASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(SourceASC->GetAnimatingAbility(), SourceHitStopTime))
		{
			PauseMontageTask->ReadyForActivation();
		}
	}
	const float TargetHitStopTime = DamageTableRow ? DamageTableRow->TargetHitStopTime : DamageGEComp->TargetHitStopTime;
	if (TargetHitStopTime > 0.f && TargetASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(TargetASC->GetAnimatingAbility(), TargetHitStopTime, TargetHitStopTime))
		{
			PauseMontageTask->ReadyForActivation();
		}
	}
}
