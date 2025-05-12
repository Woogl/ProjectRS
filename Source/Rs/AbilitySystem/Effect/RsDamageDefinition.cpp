// Copyright 2024 Team BH.


#include "RsDamageDefinition.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/System/RsDeveloperSetting.h"

void URsDamageDefinition::PostInitProperties()
{
	Super::PostInitProperties();

	// Cache a convenient pointer to the developer setting.
	DeveloperSetting = URsDeveloperSetting::Get();
}

void URsDamageDefinition::SetPierceTier(int32 InInvinciblePierce, int32 InSuperArmorPierce)
{
	InvinciblePierce = InInvinciblePierce;
	SuperArmorPierce = InSuperArmorPierce;
}

void URsDamageDefinition::SetHitReaction(FGameplayTag InHitReaction)
{
	HitReaction = InHitReaction;
}

FGameplayEffectContextHandle URsDamageDefinition::MakeDamageEffectContext(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	// Set Damage floater's location (Gameplay Cue)
	// TODO: Set HitLoc from weapon to target 
	FHitResult HitResult(TargetASC->GetAvatarActor(), nullptr, FVector(), FVector());
	EffectContext.AddHitResult(HitResult);
	return EffectContext;
}

void URsDamageDefinition::ApplyInstantDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRsEffectCoefficient& RsCoeff)
{
	if (RsCoeff.Coefficients.IsEmpty())
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle DamageSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, RsCoeff, EffectContext);
	if (DamageSpec.IsValid())
	{
		SET_BY_CALLER_PROPERTY(DamageSpec, InvinciblePierce);
		SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpec.Data, TargetASC);
	}
}

void URsDamageDefinition::ApplyDotDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRsEffectCoefficient& RsCoeff, float Duration, float Period)
{
	if (RsCoeff.Coefficients.IsEmpty())
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle DotDamageSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, RsCoeff, EffectContext);
	if (DotDamageSpec.IsValid())
	{
		DotDamageSpec.Data->SetSetByCallerMagnitude(RsGameplayTags::TAG_MANUAL_DURATION, Duration);
		DotDamageSpec.Data->Period = Period;
		SET_BY_CALLER_PROPERTY(DotDamageSpec, InvinciblePierce);
		SourceASC->ApplyGameplayEffectSpecToTarget(*DotDamageSpec.Data, TargetASC);
	}
}

void URsDamageDefinition::ApplyHitReaction(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!HitReaction.IsValid())
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	// TODO: Switch GE_HitReaction types.
	FGameplayEffectSpecHandle HitReactionSpec = SourceASC->MakeOutgoingSpec(DeveloperSetting->TriggerHitReactionEffectClass, 0.f, EffectContext);
	if (HitReactionSpec.IsValid())
	{
		SET_BY_CALLER_PROPERTY(HitReactionSpec, InvinciblePierce);
		SET_BY_CALLER_PROPERTY(HitReactionSpec, SuperArmorPierce);
		SourceASC->ApplyGameplayEffectSpecToTarget(*HitReactionSpec.Data, TargetASC);
	}
}

void URsDamageDefinition_Instant::ApplyDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC)
	{
		return;
	}
	
	FRsEffectCoefficient RsHealthDamageCoeff(DeveloperSetting->HealthDamageEffectClass, HealthDamageCoefficients);
	ApplyInstantDamage(SourceASC, TargetASC, RsHealthDamageCoeff);

	FRsEffectCoefficient RsStaggerDamageCoeff(DeveloperSetting->StaggerDamageEffectClass, StaggerDamageCoefficients);
	ApplyInstantDamage(SourceASC, TargetASC, RsStaggerDamageCoeff);

	ApplyHitReaction(SourceASC, TargetASC);
}

void URsDamageDefinition_Dot::ApplyDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC)
	{
		return;
	}
	
	FRsEffectCoefficient RsHealthDotCoeff(DeveloperSetting->HealthDotDamageEffectClass, HealthDamageCoefficients);
	ApplyDotDamage(SourceASC, TargetASC, RsHealthDotCoeff, Duration, Period);
	
	FRsEffectCoefficient RsStaggerDotCoeff(DeveloperSetting->StaggerDotDamageEffectClass, StaggerDamageCoefficients);
	ApplyDotDamage(SourceASC, TargetASC, RsStaggerDotCoeff, Duration, Period);
	
	ApplyHitReaction(SourceASC, TargetASC);
}

void URsDamageDefinition_DotBurst::ApplyDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC)
	{
		return;
	}
	
	// Apply DoT Burst Damage
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle DotBurstDamageSpec = SourceASC->MakeOutgoingSpec(DeveloperSetting->DotBurstDamageEffectClass, 0.f, EffectContext);
	if (DotBurstDamageSpec.IsValid())
	{
		SET_BY_CALLER_PROPERTY(DotBurstDamageSpec, InvinciblePierce);
		SET_BY_CALLER_PROPERTY(DotBurstDamageSpec, DamageMultiplierPerDotStacks);
		SourceASC->ApplyGameplayEffectSpecToTarget(*DotBurstDamageSpec.Data, TargetASC);
	}
	
	ApplyHitReaction(SourceASC, TargetASC);
}

void URsDamageDefinition_Custom::ApplyDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC)
	{
		return;
	}
	
	if (CustomEffect.EffectClass == nullptr)
	{
		return;
	}
	
	// Apply custom effect
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FRsEffectCoefficient CustomCoeff(CustomEffect.EffectClass, CustomEffect.Coefficients);
	FGameplayEffectSpecHandle CustomSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, CustomCoeff, EffectContext);
	if (CustomSpec.IsValid())
	{
		SET_BY_CALLER_PROPERTY(CustomSpec, InvinciblePierce);
		SourceASC->ApplyGameplayEffectSpecToTarget(*CustomSpec.Data, TargetASC);
	}
}
