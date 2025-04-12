// Copyright 2024 Team BH.


#include "RsAnimNotifyState_OnDamageAbility.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitDamageEffectApplied.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitDamageEffectBlockedImmunity.h"

void URsAnimNotifyState_OnDamageAbility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (CurrentAbility.IsValid())
	{
		// Handle damage received.
		FGameplayTargetDataFilterHandle FilterHandle;
		FGameplayTagRequirements Requirements;
		WaitAppliedTask = URsAbilityTask_WaitDamageEffectApplied::WaitDamageEffect(CurrentAbility.Get(), ReceivedDamageTags, bEnablePerfectDodgeCapsuleCollision, bTriggerOnce);
		WaitAppliedTask->OnApplied.AddDynamic(this, &ThisClass::HandleReceiveDamage);
		WaitAppliedTask->ReadyForActivation();

		// Damage can be blocked by other immnune effects.
		if (bTriggerOnDamageImmunity == true)
		{
			WaitBlockedTask = URsAbilityTask_WaitDamageEffectBlockedImmunity::WaitDamageEffectBlockedByImmunity(CurrentAbility.Get(),ReceivedDamageTags, bEnablePerfectDodgeCapsuleCollision, bTriggerOnce);
			WaitBlockedTask->Blocked.AddDynamic(this, &ThisClass::HandleBlockDamage);
			WaitBlockedTask->ReadyForActivation();
		}
	}
	
}

void URsAnimNotifyState_OnDamageAbility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (WaitAppliedTask.IsValid())
	{
		WaitAppliedTask->EndTask();
	}

	if (WaitBlockedTask.IsValid())
	{
		WaitBlockedTask->EndTask();
	}
}

void URsAnimNotifyState_OnDamageAbility::HandleReceiveDamage(AActor* Source, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle)
{
	if (OwnerASC.IsValid() && TriggeredAbilityTag.IsValid())
	{
		CurrentAbility.Get()->K2_CancelAbility();
		OwnerASC->TryActivateAbilitiesByTag(TriggeredAbilityTag.GetSingleTagContainer());
	}
}

void URsAnimNotifyState_OnDamageAbility::HandleBlockDamage(FGameplayEffectSpecHandle BlockedSpec, FActiveGameplayEffectHandle ImmunityGameplayEffectHandle)
{
	if (OwnerASC.IsValid() && TriggeredAbilityTag.IsValid())
	{
		CurrentAbility.Get()->K2_CancelAbility();
		OwnerASC->TryActivateAbilitiesByTag(TriggeredAbilityTag.GetSingleTagContainer());
	}
}
