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
	bool LocalInvinciblePierce;
	bool LocalSuperArmorPierce;
	float LocalSourceHitStopTime;
	float LocalTargetHitStopTime;
	float LocalManaGain;
	float LocalUltimateGain;
	
	if (const FRsDamageTableRow* DamageTableRow = URsAbilitySystemGlobals::GetEffectTableRow<FRsDamageTableRow>(GESpec.GetContext()))
	{
		LocalInvinciblePierce = DamageTableRow->InvinciblePierce;
		LocalSuperArmorPierce = DamageTableRow->SuperArmorPierce;
		LocalHitReaction = DamageTableRow->HitReaction;
		LocalSourceHitStopTime = DamageTableRow->SourceHitStopTime;
		LocalTargetHitStopTime = DamageTableRow->TargetHitStopTime;
		LocalManaGain = DamageTableRow->ManaGain;
		LocalUltimateGain = DamageTableRow->UltimateGain;
	}
	else
	{
		LocalInvinciblePierce = InvinciblePierce;
		LocalSuperArmorPierce = SuperArmorPierce;
		LocalHitReaction = HitReaction;
		LocalSourceHitStopTime = SourceHitStopTime;
		LocalTargetHitStopTime = TargetHitStopTime;
		LocalManaGain = ManaGain;
		LocalUltimateGain = UltimateGain;
	}

	if (LocalInvinciblePierce)
	{
		GESpec.AddDynamicAssetTag(RsGameplayTags::EFFECT_DAMAGE_INVINCIBLEPIERCE);
	}

	if (LocalSuperArmorPierce)
	{
		GESpec.AddDynamicAssetTag(RsGameplayTags::EFFECT_DAMAGE_SUPERARMORPIERCE);
	}

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
		TargetASC->HandleGameplayEvent(Payload.EventTag, &Payload);
	}

	// Trigger hit stops
	if (LocalSourceHitStopTime > 0.f && SourceASC->GetAnimatingAbility())
	{
		AActor* EffectCauser = GESpec.GetEffectContext().GetEffectCauser();
		if (!EffectCauser->IsA(ARsProjectile::StaticClass()))
		{
			if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(SourceASC->GetAnimatingAbility(), LocalSourceHitStopTime))
			{
				PauseMontageTask->ReadyForActivation();
			}
		}
	}
	if (LocalTargetHitStopTime > 0.f && TargetASC->GetAnimatingAbility())
	{
		if (URsAbilityTask_PauseMontage* PauseMontageTask = URsAbilityTask_PauseMontage::PauseMontage(TargetASC->GetAnimatingAbility(), LocalTargetHitStopTime))
		{
			PauseMontageTask->ReadyForActivation();
		}
	}

	// Advantage to damage source
	if (LocalManaGain != 0)
	{
		UGameplayEffect* GainMP = NewObject<UGameplayEffect>(GetTransientPackage(), TEXT("GainMP"));
		GainMP->DurationPolicy = EGameplayEffectDurationType::Instant;
		int32 Idx = GainMP->Modifiers.Num();
		GainMP->Modifiers.SetNum(Idx + 1);
	
		FGameplayModifierInfo& InfoMana = GainMP->Modifiers[Idx];
		InfoMana.ModifierMagnitude = FScalableFloat(LocalManaGain);
		InfoMana.ModifierOp = EGameplayModOp::Additive;
		InfoMana.Attribute = URsEnergySet::GetCurrentManaAttribute();
		
		SourceASC->ApplyGameplayEffectToSelf(GainMP, 0, SourceASC->MakeEffectContext());
	}
	
	if (LocalUltimateGain != 0)
	{
		UGameplayEffect* GainUP = NewObject<UGameplayEffect>(GetTransientPackage(), TEXT("GainUP"));
		GainUP->DurationPolicy = EGameplayEffectDurationType::Instant;
		int32 Idx = GainUP->Modifiers.Num();
		GainUP->Modifiers.SetNum(Idx + 1);
	
		FGameplayModifierInfo& InfoUP = GainUP->Modifiers[Idx];
		InfoUP.ModifierMagnitude = FScalableFloat(LocalUltimateGain);
		InfoUP.ModifierOp = EGameplayModOp::Additive;
		InfoUP.Attribute = URsEnergySet::GetCurrentUltimateAttribute();
		
		SourceASC->ApplyGameplayEffectToSelf(GainUP, 0, SourceASC->MakeEffectContext());
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
		const FString CoeffTagString = CoeffTag.ToString();
		if (CoeffTagString.IsEmpty())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag is empty"))));
			return EDataValidationResult::Invalid;
		}
		if (!CoeffTagString.StartsWith(TEXT("Coefficient.")))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag [%s] must start with \"Coefficient.\""), *CoeffTag.ToString())));
			return EDataValidationResult::Invalid;
		}
		if (!CoeffTagString.EndsWith(TEXT(".Source")) && !CoeffTagString.EndsWith(TEXT(".Target")))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag [%s] must end with \".Target\" or \".Source\""), *CoeffTag.ToString())));
			return EDataValidationResult::Invalid;
		}
	}

	for (const auto& [CoeffTag, CoeffNum] : StaggerDamageCoefficients)
	{
		if (CoeffTag == RsGameplayTags::COEFFICIENT_CONSTANT)
		{
			continue;
		}
		const FString CoeffTagString = CoeffTag.ToString();
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
