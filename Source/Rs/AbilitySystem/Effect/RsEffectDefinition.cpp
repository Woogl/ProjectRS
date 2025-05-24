// Copyright 2024 Team BH.


#include "RsEffectDefinition.h"

#include "AbilitySystemComponent.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/System/RsDeveloperSetting.h"

void URsEffectDefinition_DamageBase::PostInitProperties()
{
	Super::PostInitProperties();

	// Cache a convenient pointer to the developer setting.
	DeveloperSetting = URsDeveloperSetting::Get();
}

FGameplayEffectContextHandle URsEffectDefinition_DamageBase::MakeDamageEffectContext(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC) const
{
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	// Set Damage floater's location (Gameplay Cue)
	// TODO: Set HitLoc from weapon to target
	FHitResult HitResult(TargetASC->GetAvatarActor(), nullptr, TargetASC->GetAvatarActor()->GetActorLocation(), FVector());
	EffectContext.AddHitResult(HitResult);
	return EffectContext;
}

void URsEffectDefinition_DamageBase::ApplyInstantDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRsEffectCoefficient& RsCoeff)
{
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle DamageSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, RsCoeff, EffectContext);
	if (DamageSpec.IsValid())
	{
		SET_SETBYCALLER_PROPERTY(DamageSpec, InvinciblePierce);
		SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpec.Data, TargetASC);
	}
}

void URsEffectDefinition_DamageBase::ApplyDotDamage(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, const FRsEffectCoefficient& RsCoeff, float Duration, float Period)
{
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle DotDamageSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, RsCoeff, EffectContext);
	if (DotDamageSpec.IsValid())
	{
		DotDamageSpec.Data->SetSetByCallerMagnitude(RsGameplayTags::MANUAL_DURATION, Duration);
		DotDamageSpec.Data->Period = Period;
		SET_SETBYCALLER_PROPERTY(DotDamageSpec, InvinciblePierce);
		SourceASC->ApplyGameplayEffectSpecToTarget(*DotDamageSpec.Data, TargetASC);
	}
}

void URsEffectDefinition_DamageBase::ApplyHitReaction(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
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
		SET_SETBYCALLER_PROPERTY(HitReactionSpec, InvinciblePierce);
		SET_SETBYCALLER_PROPERTY(HitReactionSpec, SuperArmorPierce);
		SourceASC->ApplyGameplayEffectSpecToTarget(*HitReactionSpec.Data, TargetASC);
	}
}

void URsEffectDefinition_InstantDamage::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	FRsEffectCoefficient RsHealthDamageCoeff(DeveloperSetting->HealthDamageEffectClass, HealthDamageCoefficients);
	ApplyInstantDamage(SourceASC, TargetASC, RsHealthDamageCoeff);

	FRsEffectCoefficient RsStaggerDamageCoeff(DeveloperSetting->StaggerDamageEffectClass, StaggerDamageCoefficients);
	ApplyInstantDamage(SourceASC, TargetASC, RsStaggerDamageCoeff);

	ApplyHitReaction(SourceASC, TargetASC);
}

void URsEffectDefinition_DotDamage::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	FRsEffectCoefficient RsHealthDotCoeff(DeveloperSetting->HealthDotDamageEffectClass, HealthDamageCoefficients);
	ApplyDotDamage(SourceASC, TargetASC, RsHealthDotCoeff, Duration, Period);
	
	FRsEffectCoefficient RsStaggerDotCoeff(DeveloperSetting->StaggerDotDamageEffectClass, StaggerDamageCoefficients);
	ApplyDotDamage(SourceASC, TargetASC, RsStaggerDotCoeff, Duration, Period);
	
	ApplyHitReaction(SourceASC, TargetASC);
}

void URsEffectDefinition_DotBurstDamage::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	// Apply DoT Burst Damage
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle DotBurstDamageSpec = SourceASC->MakeOutgoingSpec(DeveloperSetting->DotBurstDamageEffectClass, 0.f, EffectContext);
	if (DotBurstDamageSpec.IsValid())
	{
		SET_SETBYCALLER_PROPERTY(DotBurstDamageSpec, InvinciblePierce);
		SET_SETBYCALLER_PROPERTY(DotBurstDamageSpec, DamageMultiplierPerDotStacks);
		SourceASC->ApplyGameplayEffectSpecToTarget(*DotBurstDamageSpec.Data, TargetASC);
	}
	
	ApplyHitReaction(SourceASC, TargetASC);
}

void URsEffectDefinition_Buff::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	// Apply buff effect
	FRsEffectCoefficient EffectCoefficient(BuffClass, Coefficients);
	FGameplayEffectSpecHandle BuffSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, EffectCoefficient, SourceASC->MakeEffectContext());
	if (BuffSpec.IsValid())
	{
		BuffSpec.Data->SetSetByCallerMagnitude(RsGameplayTags::MANUAL_DURATION, Duration);
		SourceASC->ApplyGameplayEffectSpecToTarget(*BuffSpec.Data, TargetASC);
	}
}

void URsEffectDefinition_Custom::ApplyEffect(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	// Apply custom effect
	FRsEffectCoefficient CustomCoeff(CustomEffect.EffectClass, CustomEffect.Coefficients);
	FGameplayEffectSpecHandle CustomSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, CustomCoeff, SourceASC->MakeEffectContext());
	if (CustomSpec.IsValid())
	{
		SourceASC->ApplyGameplayEffectSpecToTarget(*CustomSpec.Data, TargetASC);
	}
}
