// Copyright 2025 Team BH.


#include "RsDamageResponseExecution.h"

#include "AbilitySystemComponent.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"
#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"
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

    /** Response data */
	int32 SuperArmorPierce;
	FGameplayTag HitReaction;
	float SourceHitStopTime;
	float TargetHitStopTime;
	float ManaGain;
	float UltimateGain;
	TArray<TSubclassOf<UGameplayEffect>> SourceEffects;
	TArray<TSubclassOf<UGameplayEffect>> TargetEffects;

	/** Response data source */
	const FRsDamageTableRow* DamageTableRow;
	const URsDamageEffectComponent* DamageGEComp = Spec.Def->FindComponent<URsDamageEffectComponent>();
	if (DamageGEComp)
	{
		DamageTableRow = DamageGEComp->GetDamageTableRow();
	}
	else
	{
		DamageTableRow = URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsDamageTableRow>(Spec);
	}

	/** Set response data */
	if (DamageTableRow)
	{
		SuperArmorPierce = DamageTableRow->SuperArmorPierce;
		HitReaction = DamageTableRow->HitReaction;
		SourceHitStopTime = DamageTableRow->SourceHitStopTime;
		TargetHitStopTime = DamageTableRow->TargetHitStopTime;
		ManaGain = DamageTableRow->ManaGain;
		UltimateGain = DamageTableRow->UltimateGain;
		SourceEffects.Add(DamageTableRow->AdditionalSourceEffect);
		TargetEffects.Add(DamageTableRow->AdditionalTargetEffect);
	}
	else if (DamageGEComp)
	{
		SuperArmorPierce = DamageGEComp->SuperArmorPierce;
		HitReaction = DamageGEComp->HitReaction;
		SourceHitStopTime = DamageGEComp->SourceHitStopTime;
		TargetHitStopTime = DamageGEComp->TargetHitStopTime;
		ManaGain = DamageGEComp->ManaGain;
		UltimateGain = DamageGEComp->UltimateGain;
		SourceEffects = DamageGEComp->AdditionalSourceEffects;
		TargetEffects = DamageGEComp->AdditionalTargetEffects;
	}
	else
	{
		check(false);
		return;
	}

	/** Apply response data */

	// Check super armor
	float SuperArmor = URsAbilitySystemLibrary::GetNumericAttributeByTag(TargetASC, RsGameplayTags::STAT_SUA);
	if (SuperArmorPierce >= SuperArmor)
	{
		// Trigger hit reaction
		FGameplayEventData Payload;
		Payload.EventTag = HitReaction;
		Payload.Instigator = SourceASC->GetAvatarActor();
		Payload.Target = TargetASC->GetAvatarActor();
		Payload.InstigatorTags = Spec.CapturedSourceTags.GetActorTags();
		Payload.TargetTags = Spec.CapturedTargetTags.GetActorTags();
		Payload.ContextHandle = Spec.GetEffectContext();
		TargetASC->HandleGameplayEvent(Payload.EventTag, &Payload);
	}

	// Trigger hit stop
	if (SourceHitStopTime > 0.f && SourceASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(SourceASC->GetAnimatingAbility(), SourceHitStopTime))
		{
			PauseMontageTask->ReadyForActivation();
		}
	}
	if (TargetHitStopTime > 0.f && TargetASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(TargetASC->GetAnimatingAbility(), TargetHitStopTime, TargetHitStopTime))
		{
			PauseMontageTask->ReadyForActivation();
		}
	}

	// Advantage to damage source
	UGameplayEffect* GE = NewObject<UGameplayEffect>(GetTransientPackage(), TEXT("GainMP_GainUP"));
	GE->DurationPolicy = EGameplayEffectDurationType::Instant;
	int32 Idx = GE->Modifiers.Num();
	GE->Modifiers.SetNum(Idx + 2);
	
	FGameplayModifierInfo& InfoMana = GE->Modifiers[Idx];
	InfoMana.ModifierMagnitude = FScalableFloat(ManaGain);
	InfoMana.ModifierOp = EGameplayModOp::Additive;
	InfoMana.Attribute = URsEnergySet::GetCurrentManaAttribute();
	
	FGameplayModifierInfo& InfoUltimate = GE->Modifiers[Idx + 1];
	InfoUltimate.ModifierMagnitude = FScalableFloat(UltimateGain);
	InfoUltimate.ModifierOp = EGameplayModOp::Additive;
	InfoUltimate.Attribute = URsEnergySet::GetCurrentUltimateAttribute();

	SourceASC->ApplyGameplayEffectToSelf(GE, 0, SourceASC->MakeEffectContext());

	// Additional effects
	for (const TSubclassOf<UGameplayEffect>& SourceEffect : SourceEffects)
	{
		TargetASC->BP_ApplyGameplayEffectToTarget(SourceEffect, SourceASC, 0, TargetASC->MakeEffectContext());
	}
	for (const TSubclassOf<UGameplayEffect>& TargetEffect : TargetEffects)
	{
		SourceASC->BP_ApplyGameplayEffectToTarget(TargetEffect, TargetASC, 0, SourceASC->MakeEffectContext());
	}
}
