// Copyright 2025 Team BH.


#include "RsDamageResponseExecution.h"

#include "AbilitySystemComponent.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"
#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"
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
	float SuperArmor = URsAbilitySystemLibrary::GetNumericAttributeByTag(TargetASC, RsGameplayTags::STAT_SUA);
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

	// Advantage to damage source
	UGameplayEffect* GE = NewObject<UGameplayEffect>(GetTransientPackage(), TEXT("Advantage"));
	GE->DurationPolicy = EGameplayEffectDurationType::Instant;
	int32 Idx = GE->Modifiers.Num();
	GE->Modifiers.SetNum(Idx + 2);

	float ManaGain = DamageTableRow ? DamageTableRow->ManaGain : DamageGEComp->ManaGain;
	FGameplayModifierInfo& InfoMana = GE->Modifiers[Idx];
	InfoMana.ModifierMagnitude = FScalableFloat(ManaGain);
	InfoMana.ModifierOp = EGameplayModOp::Additive;
	InfoMana.Attribute = URsEnergySet::GetCurrentManaAttribute();

	float UltimateGain = DamageTableRow ? DamageTableRow->UltimateGain : DamageGEComp->UltimateGain;
	FGameplayModifierInfo& InfoUltimate = GE->Modifiers[Idx + 1];
	InfoUltimate.ModifierMagnitude = FScalableFloat(UltimateGain);
	InfoUltimate.ModifierOp = EGameplayModOp::Additive;
	InfoUltimate.Attribute = URsEnergySet::GetCurrentUltimateAttribute();

	SourceASC->ApplyGameplayEffectToSelf(GE, 0, SourceASC->MakeEffectContext());

	// Additional effects
	TArray<TSubclassOf<UGameplayEffect>> AdditionalSourceEffects = DamageTableRow ? DamageTableRow->AdditionalSourceEffects : DamageGEComp->AdditionalSourceEffects;
	for (const TSubclassOf<UGameplayEffect>& AdditionalSourceEffect : AdditionalSourceEffects)
	{
		TargetASC->BP_ApplyGameplayEffectToTarget(AdditionalSourceEffect, SourceASC, 0, TargetASC->MakeEffectContext());
	}
	
	TArray<TSubclassOf<UGameplayEffect>> AdditionalTargetEffects = DamageTableRow ? DamageTableRow->AdditionalTargetEffects : DamageGEComp->AdditionalTargetEffects;
	for (const TSubclassOf<UGameplayEffect>& AdditionalTargetEffect : AdditionalTargetEffects)
	{
		SourceASC->BP_ApplyGameplayEffectToSelf(AdditionalTargetEffect, 0, SourceASC->MakeEffectContext());
	}
}
