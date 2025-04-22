// Copyright 2024 Team BH.


#include "RsGameplayAbility_Melee.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/System/RsGameSetting.h"

void URsGameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (DamageEvents.Num() > 0)
	{
		for (const FRsDamageEventContext& Event : DamageEvents)
		{
			UAbilityTask_WaitGameplayEvent* HitDetectTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Event.DamageEventTag);
			HitDetectTask->EventReceived.AddDynamic(this, &ThisClass::HandleHitDetect);
			HitDetectTask->ReadyForActivation();
		}
	}
}

void URsGameplayAbility_Melee::HandleHitDetect(FGameplayEventData EventData)
{
	FRsDamageEventContext* DamageEffectContext = DamageEvents.FindByKey(EventData.EventTag);
	if (!DamageEffectContext->AdditionalEffectCoefficients.IsEmpty())
	{
		for (const FRsEffectCoefficient& EffectCoefficient : DamageEffectContext->AdditionalEffectCoefficients)
		{
			URsBattleLibrary::ApplyEffectCoefficient(GetAvatarActorFromActorInfo(), EventData.Target, EffectCoefficient);
			ApplyCostRecovery();
			OnAttackHitTarget(EventData.Target, EventData.EventTag);
		}
	}
}
