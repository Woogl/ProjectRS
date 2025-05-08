// Copyright 2024 Team BH.


#include "RsDamageDefinition.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/System/RsDeveloperSetting.h"

URsDamageDefinition::URsDamageDefinition()
{
	DeveloperSetting = URsDeveloperSetting::Get();
}

void URsDamageDefinition::ApplyDamageDefinition(const AActor* Source, const AActor* Target, int32 InvinciblePierce) const
{
	unimplemented();
}

void URsDamageDefinition_Instant::ApplyDamageDefinition(const AActor* Source, const AActor* Target, int32 InvinciblePierce) const
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Source);
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (SourceASC && TargetASC)
	{
		// Set Damage floater's location (Gameplay Cue)
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		EffectContext.AddOrigin(Target->GetActorLocation());
		
		// Health damage
		FGameplayEffectSpecHandle HealthDamageSpec = SourceASC->MakeOutgoingSpec(DeveloperSetting->HealthDamageEffectClass, 0.f, EffectContext);
		// Set invincible tier
		HealthDamageSpec.Data->SetSetByCallerMagnitude(RsGameplayTags::TAG_INVINCIBLEPIERCE_TIER, InvinciblePierce);
		// Set coefficient for magnitude
		for (const TTuple<FGameplayTag, float>& Coefficient : HealthDamageCoefficients)
		{
			HealthDamageSpec.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
		}
		SourceASC->ApplyGameplayEffectSpecToTarget(*HealthDamageSpec.Data, TargetASC);

		// Stagger damage
		FGameplayEffectSpecHandle StaggerDamageSpec = SourceASC->MakeOutgoingSpec(DeveloperSetting->StaggerDamageEffectClass, 0.f, EffectContext);
		// Set invincible tier
		StaggerDamageSpec.Data->SetSetByCallerMagnitude(RsGameplayTags::TAG_INVINCIBLEPIERCE_TIER, InvinciblePierce);
		// Set coefficient for magnitude
		for (const TTuple<FGameplayTag, float>& Coefficient : StaggerDamageCoefficients)
		{
			StaggerDamageSpec.Data->SetSetByCallerMagnitude(Coefficient.Key, Coefficient.Value);
		}
		SourceASC->ApplyGameplayEffectSpecToTarget(*StaggerDamageSpec.Data, TargetASC);
	}
}
