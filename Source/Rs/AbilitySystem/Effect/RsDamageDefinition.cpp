// Copyright 2024 Team BH.


#include "RsDamageDefinition.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/System/RsDeveloperSetting.h"

URsDamageDefinition::URsDamageDefinition()
{
	DeveloperSetting = URsDeveloperSetting::Get();
}

void URsDamageDefinition::SetInvinciblePierce(int32 InInvinciblePierce)
{
	InvinciblePierce = InInvinciblePierce;
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
	
	// Set Damage floater's location (Gameplay Cue)
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	FHitResult HitResult(TargetASC->GetAvatarActor(), nullptr, TargetASC->GetAvatarActor()->GetActorLocation(), FVector());
	EffectContext.AddHitResult(HitResult);
		
	// Health damage
	FGameplayEffectSpecHandle HealthDamageSpec = SourceASC->MakeOutgoingSpec(DeveloperSetting->HealthDamageEffectClass, 0.f, EffectContext);
	// Set coefficient for magnitude
	for (const TTuple<FGameplayTag, float>& Coefficient : HealthDamageCoefficients)
	{
		HealthDamageSpec.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
	}
	SET_BY_CALLER_PROPERTY(HealthDamageSpec, InvinciblePierce);
	SourceASC->ApplyGameplayEffectSpecToTarget(*HealthDamageSpec.Data, TargetASC);

	// Stagger damage
	FGameplayEffectSpecHandle StaggerDamageSpec = SourceASC->MakeOutgoingSpec(DeveloperSetting->StaggerDamageEffectClass, 0.f, EffectContext);
	// Set coefficient for magnitude
	for (const TTuple<FGameplayTag, float>& Coefficient : StaggerDamageCoefficients)
	{
		StaggerDamageSpec.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
	}
	SET_BY_CALLER_PROPERTY(StaggerDamageSpec, InvinciblePierce);
	SourceASC->ApplyGameplayEffectSpecToTarget(*StaggerDamageSpec.Data, TargetASC);
}

void URsDamageDefinition_Dot::ApplyDamageDefinition(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	if (!SourceASC || !TargetASC)
	{
		return;
	}

	// Set Damage floater's location (Gameplay Cue)
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	FHitResult HitResult(TargetASC->GetAvatarActor(), nullptr, TargetASC->GetAvatarActor()->GetActorLocation(), FVector());
	EffectContext.AddHitResult(HitResult);
		
	// Health DoT damage
	FGameplayEffectSpecHandle HealthDotDamageSpec = SourceASC->MakeOutgoingSpec(DeveloperSetting->HealthDotDamageEffectClass, 0.f, EffectContext);
	// Set coefficient for magnitude
	for (const TTuple<FGameplayTag, float>& Coefficient : HealthDamageCoefficients)
	{
		HealthDotDamageSpec.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
	}
	HealthDotDamageSpec.Data->SetSetByCallerMagnitude(RsGameplayTags::TAG_MANUAL_DURATION, Duration);
	HealthDotDamageSpec.Data->Period = Period;
	SET_BY_CALLER_PROPERTY(HealthDotDamageSpec, InvinciblePierce);
	SourceASC->ApplyGameplayEffectSpecToTarget(*HealthDotDamageSpec.Data, TargetASC);

	// Stagger damage
	FGameplayEffectSpecHandle StaggerDotDamageSpec = SourceASC->MakeOutgoingSpec(DeveloperSetting->StaggerDotDamageEffectClass, 0.f, EffectContext);
	// Set coefficient for magnitude
	for (const TTuple<FGameplayTag, float>& Coefficient : StaggerDamageCoefficients)
	{
		StaggerDotDamageSpec.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
	}
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
	
	// Set Damage floater's location (Gameplay Cue)
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	FHitResult HitResult(TargetASC->GetAvatarActor(), nullptr, TargetASC->GetAvatarActor()->GetActorLocation(), FVector());
	EffectContext.AddHitResult(HitResult);
	
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
	
	// Set Damage floater's location (Gameplay Cue)
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	FHitResult HitResult(TargetASC->GetAvatarActor(), nullptr, TargetASC->GetAvatarActor()->GetActorLocation(), FVector());
	EffectContext.AddHitResult(HitResult);

	// Set coefficient for magnitude
	FGameplayEffectSpecHandle CustomEffectSpec = SourceASC->MakeOutgoingSpec(CustomEffect.EffectClass, 0, EffectContext);
	for (const TTuple<FGameplayTag, float>& Coefficient : CustomEffect.Coefficients)
	{
		CustomEffectSpec.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
	}
	SET_BY_CALLER_PROPERTY(CustomEffectSpec, InvinciblePierce);
	SourceASC->ApplyGameplayEffectSpecToTarget(*CustomEffectSpec.Data, TargetASC);
}
