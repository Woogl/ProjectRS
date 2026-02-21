// Copyright 2025 Team BH.


#include "RsDamageEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Misc/DataValidation.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"
#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"
#include "Rs/Battle/Actor/RsProjectile.h"

URsDamageEffectComponent::URsDamageEffectComponent()
{
}

void URsDamageEffectComponent::OnGameplayEffectChanged()
{
	Super::OnGameplayEffectChanged();
}

void URsDamageEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);

	UAbilitySystemComponent* SourceASC = GESpec.GetContext().GetInstigatorAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ActiveGEContainer.Owner;
	if (!TargetASC)
	{
		return;
	}
	
	const FRsDamageTableRow* DamageTableRow = URsAbilitySystemGlobals::GetEffectTableRow<FRsDamageTableRow>(GESpec.GetContext());
	if (!DamageTableRow)
	{
		return;
	}

	if (DamageTableRow->InvinciblePierce)
	{
		GESpec.AddDynamicAssetTag(RsGameplayTags::EFFECT_DAMAGE_INVINCIBLEPIERCE);
	}

	if (DamageTableRow->SuperArmorPierce)
	{
		GESpec.AddDynamicAssetTag(RsGameplayTags::EFFECT_DAMAGE_SUPERARMORPIERCE);
	}

	// Trigger hit reaction
	if (DamageTableRow->HitReaction.IsValid())
	{
		FGameplayEventData Payload;
		Payload.EventTag = DamageTableRow->HitReaction;
		Payload.Instigator = SourceASC->GetAvatarActor();
		Payload.Target = TargetASC->GetAvatarActor();
		Payload.InstigatorTags = *GESpec.CapturedSourceTags.GetAggregatedTags();
		Payload.TargetTags = *GESpec.CapturedTargetTags.GetAggregatedTags();
		Payload.ContextHandle = GESpec.GetEffectContext();
		TargetASC->HandleGameplayEvent(Payload.EventTag, &Payload);
	}

	// Trigger hit stops
	if (DamageTableRow->SourceHitStopTime > 0.f && SourceASC->GetAnimatingAbility())
	{
		AActor* EffectCauser = GESpec.GetEffectContext().GetEffectCauser();
		if (!EffectCauser->IsA(ARsProjectile::StaticClass()))
		{
			if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(SourceASC->GetAnimatingAbility(), DamageTableRow->SourceHitStopTime))
			{
				PauseMontageTask->ReadyForActivation();
			}
		}
	}
	if (DamageTableRow->TargetHitStopTime > 0.f && TargetASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(TargetASC->GetAnimatingAbility(), DamageTableRow->TargetHitStopTime))
		{
			PauseMontageTask->ReadyForActivation();
		}
	}

	// Advantage to damage source
	if (DamageTableRow->ManaGain != 0)
	{
		UGameplayEffect* GainMP = NewObject<UGameplayEffect>(GetTransientPackage(), TEXT("GainMP"));
		GainMP->DurationPolicy = EGameplayEffectDurationType::Instant;
		int32 Idx = GainMP->Modifiers.Num();
		GainMP->Modifiers.SetNum(Idx + 1);
	
		FGameplayModifierInfo& InfoMana = GainMP->Modifiers[Idx];
		InfoMana.ModifierMagnitude = FScalableFloat(DamageTableRow->ManaGain);
		InfoMana.ModifierOp = EGameplayModOp::Additive;
		InfoMana.Attribute = URsEnergySet::GetCurrentManaAttribute();
		
		SourceASC->ApplyGameplayEffectToSelf(GainMP, 0, SourceASC->MakeEffectContext());
	}
	
	if (DamageTableRow->UltimateGain != 0)
	{
		UGameplayEffect* GainUP = NewObject<UGameplayEffect>(GetTransientPackage(), TEXT("GainUP"));
		GainUP->DurationPolicy = EGameplayEffectDurationType::Instant;
		int32 Idx = GainUP->Modifiers.Num();
		GainUP->Modifiers.SetNum(Idx + 1);
	
		FGameplayModifierInfo& InfoUP = GainUP->Modifiers[Idx];
		InfoUP.ModifierMagnitude = FScalableFloat(DamageTableRow->UltimateGain);
		InfoUP.ModifierOp = EGameplayModOp::Additive;
		InfoUP.Attribute = URsEnergySet::GetCurrentUltimateAttribute();
		
		SourceASC->ApplyGameplayEffectToSelf(GainUP, 0, SourceASC->MakeEffectContext());
	}
}

#if WITH_EDITOR
EDataValidationResult URsDamageEffectComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	return Result;
}
#endif // WITH_EDITOR
