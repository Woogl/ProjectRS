// Copyright 2024 Team BH.


#include "RsDamageDefinition.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/System/RsDeveloperSetting.h"

URsDamageDefinition::URsDamageDefinition()
{
	DeveloperSetting = URsDeveloperSetting::Get();
}

void URsDamageDefinition::SetInvinciblePierce(int32 InInvinciblePierce)
{
	InvinciblePierce = InInvinciblePierce;
}

FGameplayEffectContextHandle URsDamageDefinition::MakeDamageEffectContext(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	// Set Damage floater's location (Gameplay Cue)
	FHitResult HitResult(TargetASC->GetAvatarActor(), nullptr, TargetASC->GetAvatarActor()->GetActorLocation(), FVector());
	EffectContext.AddHitResult(HitResult);
	return EffectContext;
}

void URsDamageDefinition::ApplyDamageDefinition(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	unimplemented();
}

void URsDamageDefinition_Instant::ApplyDamageDefinition(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	
	// Health damage
	FRsEffectCoefficient RsHealthCoefficient(DeveloperSetting->HealthDamageEffectClass, HealthDamageCoefficients);
	FGameplayEffectSpecHandle HealthDamageSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, RsHealthCoefficient, EffectContext);
	SET_BY_CALLER_PROPERTY(HealthDamageSpec, InvinciblePierce);
	SourceASC->ApplyGameplayEffectSpecToTarget(*HealthDamageSpec.Data, TargetASC);

	// Stagger damage
	FRsEffectCoefficient RsStaggerCoefficient(DeveloperSetting->StaggerDamageEffectClass, StaggerDamageCoefficients);
	FGameplayEffectSpecHandle StaggerDamageSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, RsStaggerCoefficient, EffectContext);
	SET_BY_CALLER_PROPERTY(StaggerDamageSpec, InvinciblePierce);
	SourceASC->ApplyGameplayEffectSpecToTarget(*StaggerDamageSpec.Data, TargetASC);
}

void URsDamageDefinition_Dot::ApplyDamageDefinition(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	
	// Health DoT damage
	FRsEffectCoefficient RsHealthDotCoefficient(DeveloperSetting->HealthDotDamageEffectClass, HealthDamageCoefficients);
	FGameplayEffectSpecHandle HealthDotDamageSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, RsHealthDotCoefficient, EffectContext);
	HealthDotDamageSpec.Data->SetSetByCallerMagnitude(RsGameplayTags::TAG_MANUAL_DURATION, Duration);
	HealthDotDamageSpec.Data->Period = Period;
	SET_BY_CALLER_PROPERTY(HealthDotDamageSpec, InvinciblePierce);
	SourceASC->ApplyGameplayEffectSpecToTarget(*HealthDotDamageSpec.Data, TargetASC);
	
	// Stagger damage
	FRsEffectCoefficient RsStaggerDotCoefficient(DeveloperSetting->StaggerDotDamageEffectClass, StaggerDamageCoefficients);
	FGameplayEffectSpecHandle StaggerDotDamageSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, RsStaggerDotCoefficient, EffectContext);
	StaggerDotDamageSpec.Data->SetSetByCallerMagnitude(RsGameplayTags::TAG_MANUAL_DURATION, Duration);
	StaggerDotDamageSpec.Data->Period = Period;
	SET_BY_CALLER_PROPERTY(StaggerDotDamageSpec, InvinciblePierce);
	SourceASC->ApplyGameplayEffectSpecToTarget(*StaggerDotDamageSpec.Data, TargetASC);
}

void URsDamageDefinition_DotBurst::ApplyDamageDefinition(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC)
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = MakeDamageEffectContext(SourceASC, TargetASC);
	FGameplayEffectSpecHandle DotBurstDamageSpec = SourceASC->MakeOutgoingSpec(DeveloperSetting->DotBurstDamageEffectClass, 0.f, EffectContext);
	SET_BY_CALLER_PROPERTY(DotBurstDamageSpec, InvinciblePierce);
	SET_BY_CALLER_PROPERTY(DotBurstDamageSpec, DamageMultiplierPerDotStacks);
	SourceASC->ApplyGameplayEffectSpecToTarget(*DotBurstDamageSpec.Data, TargetASC);
}

void URsDamageDefinition_Custom::ApplyDamageDefinition(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC)
	{
		return;
	}
	
	if (CustomEffect.EffectClass == nullptr)
	{
		return;
	}

	FRsEffectCoefficient RsCustomCoefficient(CustomEffect.EffectClass, CustomEffect.Coefficients);
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	FGameplayEffectSpecHandle CustomEffectSpec = URsBattleLibrary::MakeEffectSpecCoefficient(SourceASC, RsCustomCoefficient, EffectContext);
	SET_BY_CALLER_PROPERTY(CustomEffectSpec, InvinciblePierce);
	SourceASC->ApplyGameplayEffectSpecToTarget(*CustomEffectSpec.Data, TargetASC);
}
