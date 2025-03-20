// Copyright 2024 Team BH.


#include "RsGameplayAbility_Melee.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/System/RsGameSetting.h"

void URsGameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	TArray<FGameplayTag> EventTags;
	DamageEvents.GetKeys(EventTags);
	if (EventTags.Num() > 0)
	{
		for (const FGameplayTag& EventTag : EventTags)
		{
			UAbilityTask_WaitGameplayEvent* HitDetectTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag);
			HitDetectTask->EventReceived.AddDynamic(this, &ThisClass::HandleHitDetect);
			HitDetectTask->ReadyForActivation();
		}
	}
}

void URsGameplayAbility_Melee::HandleHitDetect(FGameplayEventData EventData)
{
	FRsDamageEventContext DamageEffectContext = DamageEvents.FindRef(EventData.EventTag);
	if (!DamageEffectContext.DamageEffectParams.IsEmpty())
	{
		TArray<FGameplayEffectSpecHandle> DamageEffectHandles = URsBattleLibrary::MakeDamageEffectSpecs(GetAvatarActorFromActorInfo(), DamageEffectContext.DamageEffectParams);

		// Todo : Set Magnitude only for Damage GE (may need CoefficientRequired GE, LevelRequired GE Class)
		for (const FGameplayEffectSpecHandle& DamageEffectHandle : DamageEffectHandles)
		{
			DamageEffectHandle.Data->SetSetByCallerMagnitude(URsGameSetting::Get()->HealthDamageCoefficientTag,DamageEffectContext.HealthDamageCoefficient);
			DamageEffectHandle.Data->SetSetByCallerMagnitude(URsGameSetting::Get()->StaggerDamageCoefficientTag,DamageEffectContext.StaggerDamageCoefficient);
			
			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageEffectHandle.Data, UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EventData.Target));
		}
		ApplyCostRecovery();
		K2_OnAttackHitTarget(EventData.Target, EventData.EventTag);
	}
}
