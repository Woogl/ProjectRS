// Copyright 2024 Team BH.


#include "RsGameplayAbility_Melee.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Rs/Battle/RsBattleLibrary.h"

void URsGameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (DamageEvents.Num() > 0)
	{
		for (const FRsDamageContext& Event : DamageEvents)
		{
			UAbilityTask_WaitGameplayEvent* HitDetectTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Event.DamageEventTag);
			HitDetectTask->EventReceived.AddDynamic(this, &ThisClass::HandleHitDetect);
			HitDetectTask->ReadyForActivation();
		}
	}
}

void URsGameplayAbility_Melee::HandleHitDetect(FGameplayEventData EventData)
{
	FRsDamageContext* DamageContext = DamageEvents.FindByKey(EventData.EventTag);
	if (!DamageContext->EffectCoefficients.IsEmpty())
	{
		URsBattleLibrary::ApplyDamageContext(GetAvatarActorFromActorInfo(), EventData.Target, *DamageContext);
		ApplyCostRecovery();
		OnAttackHitTarget(EventData.Target, EventData.EventTag);
	}
}
