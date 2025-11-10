// Copyright 2025 Team BH.


#include "RsDamageEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Misc/DataValidation.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_PauseMontage.h"
#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffect.h"

URsDamageEffectComponent::URsDamageEffectComponent()
{
	HealthDamageCoefficients.Add(RsGameplayTags::COEFFICIENT_ATK_SOURCE, 1.f);
	StaggerDamageCoefficients.Add(RsGameplayTags::COEFFICIENT_IMP_SOURCE, 1.f);
	DamageTags.AddTag(RsGameplayTags::EFFECT_DAMAGE);
}

void URsDamageEffectComponent::OnGameplayEffectChanged()
{
	Super::OnGameplayEffectChanged();
	
	UGameplayEffect* Owner = GetOwner();
	Owner->CachedAssetTags.AppendTags(DamageTags);
	Owner->CachedGrantedTags.AppendTags(DamageTags);
}

bool URsDamageEffectComponent::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	bool bResult;
	float StatValue = URsAbilitySystemLibrary::GetNumericAttributeByTag(ActiveGEContainer.Owner, RsGameplayTags::STAT_INV);
	if (const FRsDamageTableRow* DamageTableRow = GetDamageTableRow(GESpec))
	{
		bResult = StatValue <= DamageTableRow->InvinciblePierce;
	}
	else
	{
		bResult = StatValue <= InvinciblePierce;
	}
	if (!bResult)
	{
		ActiveGEContainer.Owner->OnImmunityBlockGameplayEffectDelegate.Broadcast(GESpec, nullptr);
	}
	return bResult;
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
	
	FGameplayTag LocalHitReaction;
	int32 LocalSuperArmorPierce;
	float LocalSourceHitStopTime;
	float LocalTargetHitStopTime;
	float LocalManaGain;
	float LocalUltimateGain;
	TArray<TSubclassOf<UGameplayEffect>> LocalAdditionalSourceEffects;
	TArray<TSubclassOf<UGameplayEffect>> LocalAdditionalTargetEffects;
	
	if (const FRsDamageTableRow* DamageTableRow = GetDamageTableRow(GESpec))
	{
		LocalSuperArmorPierce = DamageTableRow->SuperArmorPierce;
		LocalHitReaction = DamageTableRow->HitReaction;
		LocalSourceHitStopTime = DamageTableRow->SourceHitStopTime;
		LocalTargetHitStopTime = DamageTableRow->TargetHitStopTime;
		LocalManaGain = DamageTableRow->ManaGain;
		LocalUltimateGain = DamageTableRow->UltimateGain;
		// TODO: LocalAdditionalSourceEffects
		// TODO: LocalAdditionalTargetEffects
	}
	else
	{
		LocalSuperArmorPierce = SuperArmorPierce;
		LocalHitReaction = HitReaction;
		LocalSourceHitStopTime = SourceHitStopTime;
		LocalTargetHitStopTime = TargetHitStopTime;
		LocalManaGain = ManaGain;
		LocalUltimateGain = UltimateGain;
		LocalAdditionalSourceEffects = AdditionalSourceEffects;
		LocalAdditionalTargetEffects = AdditionalTargetEffects;
	}

	// Check super armor
	float TargetSuperArmor = URsAbilitySystemLibrary::GetNumericAttributeByTag(TargetASC, RsGameplayTags::STAT_SUA);
	if (LocalSuperArmorPierce >= TargetSuperArmor)
	{
		// Trigger hit reaction
		if (LocalHitReaction.IsValid())
		{
			FGameplayEventData Payload;
			Payload.EventTag = LocalHitReaction;
			Payload.Instigator = SourceASC->GetAvatarActor();
			Payload.Target = TargetASC->GetAvatarActor();
			Payload.InstigatorTags = GESpec.CapturedSourceTags.GetActorTags();
			Payload.TargetTags = GESpec.CapturedTargetTags.GetActorTags();
			Payload.ContextHandle = GESpec.GetEffectContext();
			TargetASC->HandleGameplayEvent(LocalHitReaction, &Payload);
		}
	}

	// Trigger hit stops
	if (LocalSourceHitStopTime > 0.f && SourceASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(SourceASC->GetAnimatingAbility(), LocalSourceHitStopTime))
		{
			PauseMontageTask->ReadyForActivation();
		}
	}
	if (LocalTargetHitStopTime > 0.f && TargetASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(TargetASC->GetAnimatingAbility(), LocalTargetHitStopTime, LocalTargetHitStopTime))
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
	InfoMana.ModifierMagnitude = FScalableFloat(LocalManaGain);
	InfoMana.ModifierOp = EGameplayModOp::Additive;
	InfoMana.Attribute = URsEnergySet::GetCurrentManaAttribute();
	
	FGameplayModifierInfo& InfoUltimate = GE->Modifiers[Idx + 1];
	InfoUltimate.ModifierMagnitude = FScalableFloat(LocalUltimateGain);
	InfoUltimate.ModifierOp = EGameplayModOp::Additive;
	InfoUltimate.Attribute = URsEnergySet::GetCurrentUltimateAttribute();
	
	SourceASC->ApplyGameplayEffectToSelf(GE, 0, SourceASC->MakeEffectContext());

	// Additional effects
	for (const TSubclassOf<UGameplayEffect>& SourceEffect : LocalAdditionalSourceEffects)
	{
		TargetASC->BP_ApplyGameplayEffectToTarget(SourceEffect, SourceASC, GESpec.GetLevel(), TargetASC->MakeEffectContext());
	}
	for (const TSubclassOf<UGameplayEffect>& TargetEffect : LocalAdditionalTargetEffects)
	{
		SourceASC->BP_ApplyGameplayEffectToTarget(TargetEffect, TargetASC, GESpec.GetLevel(), SourceASC->MakeEffectContext());
	}
}

#if WITH_EDITOR
EDataValidationResult URsDamageEffectComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	// Check asset data
	for (const auto& [CoeffTag, CoeffNum] : HealthDamageCoefficients)
	{
		if (CoeffTag == RsGameplayTags::COEFFICIENT_CONSTANT)
		{
			continue;
		}
		FString CoeffTagString = CoeffTag.ToString();
		if (CoeffTagString.IsEmpty())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag is empty"))));
			return EDataValidationResult::Invalid;
		}
		if (!CoeffTagString.StartsWith(TEXT("Coefficient.")))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag { %s } must start with \"Coefficient.\""), *CoeffTag.ToString())));
			return EDataValidationResult::Invalid;
		}
		if (!CoeffTagString.EndsWith(TEXT(".Source")) && !CoeffTagString.EndsWith(TEXT(".Target")))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag { %s } must end with \".Target\" or \".Source\""), *CoeffTag.ToString())));
			return EDataValidationResult::Invalid;
		}
	}

	for (const auto& [CoeffTag, CoeffNum] : StaggerDamageCoefficients)
	{
		if (CoeffTag == RsGameplayTags::COEFFICIENT_CONSTANT)
		{
			continue;
		}
		FString CoeffTagString = CoeffTag.ToString();
		if (CoeffTagString.IsEmpty())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag is empty"))));
			return EDataValidationResult::Invalid;
		}
		if (!CoeffTagString.StartsWith(TEXT("Coefficient.")))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag \"%s\" must start with \"Coefficient.\""), *CoeffTag.ToString())));
			return EDataValidationResult::Invalid;
		}
		if (!CoeffTagString.EndsWith(TEXT(".Source")) && !CoeffTagString.EndsWith(TEXT(".Target")))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag \"%s\" must end with \".Target\" or \".Source\""), *CoeffTag.ToString())));
			return EDataValidationResult::Invalid;
		}
	}
	return Result;
}
#endif // WITH_EDITOR

const FRsDamageTableRow* URsDamageEffectComponent::GetDamageTableRow(const FGameplayEffectSpec& GESpec) const
{
	return URsAbilitySystemGlobals::GetSetByCallerTableRow<FRsDamageTableRow>(GESpec);
}
