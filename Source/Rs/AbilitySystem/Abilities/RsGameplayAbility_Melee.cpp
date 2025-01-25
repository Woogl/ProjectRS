// Copyright 2024 Team BH.


#include "RsGameplayAbility_Melee.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_TurnToLocation.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/System/RsGameplayTags.h"

void URsGameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (FocusTargetingPreset)
	{
		TArray<AActor*> Victims;
		if (URsBattleLibrary::ExecuteTargeting(GetAvatarActorFromActorInfo(), FocusTargetingPreset, Victims))
		{
			URsAbilityTask_TurnToLocation* TurnTask = URsAbilityTask_TurnToLocation::TurnToLocation(this, Victims[0]->GetActorLocation(), RotatingSpeed, RotatingMaxDuration);
			TurnTask->ReadyForActivation();
		}
	}
	
	if (DamageTargetingPreset)
	{
		if (HitDetectEventTag.IsValid())
		{
			UAbilityTask_WaitGameplayEvent* HitDetectTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, HitDetectEventTag);
			HitDetectTask->EventReceived.AddDynamic(this, &ThisClass::HandleHitDetect);
			HitDetectTask->ReadyForActivation();
		}
		else
		{
			// Deal damage immediately if Event tag is not set.
			HandleHitDetect(FGameplayEventData());
		}
	}
}

void URsGameplayAbility_Melee::HandleHitDetect(FGameplayEventData EventData)
{
	TArray<AActor*> Victims;
	if (URsBattleLibrary::ExecuteTargeting(GetAvatarActorFromActorInfo(), DamageTargetingPreset, Victims))
	{
		for (AActor* Victim : Victims)
		{
			FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
			if (DamageEffectSpecHandle.IsValid())
			{
				DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(RsGameplayTags::TAG_COEFFICIENT_DAMAGE_HEALTH, HealthDamageCoefficient);
				DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(RsGameplayTags::TAG_COEFFICIENT_DAMAGE_STAGGER, StaggerDamageCoefficient);
				URsBattleLibrary::ApplyDamageEffectSpec(GetAvatarActorFromActorInfo(), Victim, DamageEffectSpecHandle);
			}
		}
		ApplyCostRecoveryEffect();
	}
}
