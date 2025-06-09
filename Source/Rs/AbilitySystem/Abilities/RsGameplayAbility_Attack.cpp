// Copyright 2024 Team BH.


#include "RsGameplayAbility_Attack.h"

#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_TurnToLocation.h"
#include "Rs/AbilitySystem/Effect/RsEffectDefinition.h"
#include "Rs/Battle/RsBattleLibrary.h"
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

FRsDamageContext* URsGameplayAbility_Attack::FindDamageEvent(FGameplayTag EventTag)
{
	return DamageEvents.FindByKey(EventTag);
}

void URsGameplayAbility_Attack::ApplyDamageEvent(FGameplayTag EventTag, const AActor* Victim)
{
	if (FRsDamageContext* DamageContext = FindDamageEvent(EventTag))
	{
		UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetAvatarActorFromActorInfo());
		UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Victim);
		if (!SourceASC || !TargetASC)
		{
			return;
		}

		// Can be immune by RsInvincibleGEComp or RsSuperArmorGECpomp.
		for (TObjectPtr<URsEffectDefinition> DamageDefinition : DamageContext->DamageDefinitions)
		{
			DamageDefinition->ApplyEffect(SourceASC, TargetASC);
		}

		// Post damage
		StatesContainer->SetValue<bool>(FName("HasHitTarget"), true);
		ApplyCostRecovery();
		OnAttackHitTarget.Broadcast(Victim, EventTag);

		// TODO: Delete this
		K2_OnAttackHitTarget(Victim, EventTag);
	}
}
