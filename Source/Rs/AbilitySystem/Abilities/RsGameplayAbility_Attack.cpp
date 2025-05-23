// Copyright 2024 Team BH.


#include "RsGameplayAbility_Attack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_TurnToLocation.h"
#include "Rs/System/RsGenericContainer.h"

void URsGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
}

void URsGameplayAbility_Attack::HandleMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URsGameplayAbility_Attack::HandleMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URsGameplayAbility_Attack::OnAttackHitTarget(const AActor* Target, const FGameplayTag& DamageEvent)
{
	StatesContainer->SetValue<bool>(FName("HasHitTarget"), true);
	ApplyCostRecovery();
	K2_OnAttackHitTarget(Target, DamageEvent);
}

FRsDamageContext* URsGameplayAbility_Attack::FindDamageEvent(FGameplayTag EventTag)
{
	return DamageEvents.FindByKey(EventTag);
}
