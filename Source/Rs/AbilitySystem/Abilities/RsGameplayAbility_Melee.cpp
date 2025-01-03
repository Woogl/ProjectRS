// Copyright 2024 Team BH.


#include "RsGameplayAbility_Melee.h"

#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Rs/Battle/RsBattleLibrary.h"

void URsGameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (MontageToPlay)
	{
		if (UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay))
		{
			MontageTask->OnCompleted.AddDynamic(this, &ThisClass::HandleMontageEnd);
			MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::HandleMontageEnd);
			MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::HandleMontageEnd);
			MontageTask->OnCancelled.AddDynamic(this, &ThisClass::HandleMontageEnd);
			MontageTask->Activate();
		}
	}
	
	if (HitDetectEventTag.IsValid())
	{
		if (UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, HitDetectEventTag))
		{
			GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::HandleHitDetectEvent);
			GameplayEventTask->Activate();
		}
	}
}

void URsGameplayAbility_Melee::HandleMontageEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URsGameplayAbility_Melee::HandleHitDetectEvent(FGameplayEventData EventData)
{
	TArray<AActor*> Victims = URsBattleLibrary::PerformTargeting(GetAvatarActorFromActorInfo(), HitDetectTargetingPreset);
	for (AActor* Victim : Victims)
	{
		URsBattleLibrary::ApplyDamageEffect(GetAvatarActorFromActorInfo(), Victim, DamageEffect);
	}
}
