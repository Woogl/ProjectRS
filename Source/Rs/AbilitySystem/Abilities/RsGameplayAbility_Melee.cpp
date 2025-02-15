// Copyright 2024 Team BH.


#include "RsGameplayAbility_Melee.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/System/RsGameSetting.h"

void URsGameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (HitDetectEventTag != FGameplayTag::EmptyTag)
	{
		UAbilityTask_WaitGameplayEvent* HitDetectTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, HitDetectEventTag);
		HitDetectTask->EventReceived.AddDynamic(this, &ThisClass::HandleHitDetect);
		HitDetectTask->ReadyForActivation();
	}
}

void URsGameplayAbility_Melee::HandleHitDetect(FGameplayEventData EventData)
{
	FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	if (DamageEffectSpecHandle.IsValid())
	{
		DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(URsGameSetting::Get()->HealthDamageCoefficientTag, HealthDamageCoefficient);
		DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(URsGameSetting::Get()->StaggerDamageCoefficientTag, StaggerDamageCoefficient);
		URsBattleLibrary::ApplyDamageEffectSpec(GetAvatarActorFromActorInfo(), EventData.Target, DamageEffectSpecHandle);
		K2_OnAttackHitTarget(EventData.Target);
	}
}
