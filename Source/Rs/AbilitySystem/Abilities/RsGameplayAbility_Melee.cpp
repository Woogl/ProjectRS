// Copyright 2024 Team BH.


#include "RsGameplayAbility_Melee.h"

#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_TurnToLocation.h"
#include "Rs/Battle/RsBattleLibrary.h"

void URsGameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (MontageToPlay)
	{
		if (UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay))
		{
			MontageTask->OnCompleted.AddDynamic(this, &ThisClass::HandleMontageCompleted);
			MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::HandleMontageCompleted);
			MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::HandleMontageCancelled);
			MontageTask->OnCancelled.AddDynamic(this, &ThisClass::HandleMontageCancelled);
			MontageTask->ReadyForActivation();
		}
	}

	if (FocusTargetingPreset)
	{
		bool bFound = false;
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
			HitDetectTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, HitDetectEventTag);
			HitDetectTask->EventReceived.AddDynamic(this, &ThisClass::HandleHitDetectEvent);
			HitDetectTask->ReadyForActivation();
		}
		else
		{
			// Deal damage immediately if Event tag is not set.
			HandleHitDetectEvent(FGameplayEventData());
		}
	}
}

void URsGameplayAbility_Melee::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (HitDetectTask)
	{
		HitDetectTask->EndTask();
		HitDetectTask = nullptr;
	}
}

void URsGameplayAbility_Melee::HandleMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URsGameplayAbility_Melee::HandleMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URsGameplayAbility_Melee::HandleHitDetectEvent(FGameplayEventData EventData)
{
	bool bFound = false;
	TArray<AActor*> Victims;
	if (URsBattleLibrary::ExecuteTargeting(GetAvatarActorFromActorInfo(), DamageTargetingPreset, Victims))
	{
		for (AActor* Victim : Victims)
		{
			URsBattleLibrary::ApplyDamageEffect(GetAvatarActorFromActorInfo(), Victim, DamageEffect);
		}
	}
}
