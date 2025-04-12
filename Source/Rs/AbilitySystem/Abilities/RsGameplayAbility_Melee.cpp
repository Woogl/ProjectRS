// Copyright 2024 Team BH.


#include "RsGameplayAbility_Melee.h"

#include "AbilitySystemComponent.h"
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
	if (DamageEffectContext.DamageEffectClass != nullptr)
	{
		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectContext.DamageEffectClass, GetAbilityLevel());
		if (DamageEffectSpecHandle.IsValid())
		{
			DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(URsGameSetting::Get()->HealthDamageCoefficientTag, DamageEffectContext.HealthDamageCoefficient);
			DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(URsGameSetting::Get()->StaggerDamageCoefficientTag, DamageEffectContext.StaggerDamageCoefficient);
			URsBattleLibrary::ApplyDamageEffectSpec(GetAvatarActorFromActorInfo(), EventData.Target, DamageEffectSpecHandle, DamageEffectContext.DamageEffectTags);
			ApplyCostRecovery();
			OnAttackHitTarget(EventData.Target, EventData.EventTag);
		}
	}
}
