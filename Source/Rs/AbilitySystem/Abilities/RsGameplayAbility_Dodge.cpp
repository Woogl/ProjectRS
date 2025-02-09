// Copyright 2024 Team BH.


#include "RsGameplayAbility_Dodge.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectApplied_Self.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_TurnToLocation.h"
#include "Rs/Character/RsCharacterBase.h"

void URsGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FVector LastMoveInput = GetAvatarCharacter()->GetLastMovementInputVector();
	MontageToPlay = LastMoveInput.IsNearlyZero() ? BackstepMontage : DashMontage;
	
	if (!LastMoveInput.IsNearlyZero())
	{
		FVector TargetLocation = GetAvatarCharacter()->GetActorLocation() + LastMoveInput * 100.f;
		URsAbilityTask_TurnToLocation* TurnTask = URsAbilityTask_TurnToLocation::TurnToLocation(this, TargetLocation, InputTurnAroundSpeed, InputTurnAroundMaxDuration);
		// Reserve Dash montage
		TurnTask->OnFinish.AddDynamic(this, &ThisClass::PlayDashOrBackstepMontage);
		TurnTask->ReadyForActivation();
	}
	else
	{
		// Play Backstep montage
		PlayDashOrBackstepMontage();
	}
	
	if (PerfectDodgeStartTag != FGameplayTag::EmptyTag)
	{
		UAbilityTask_WaitGameplayEvent* PerfectDodgeStartTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PerfectDodgeStartTag);
		PerfectDodgeStartTask->EventReceived.AddDynamic(this, &ThisClass::HandlePerfectDodgeStarted);
		PerfectDodgeStartTask->ReadyForActivation();
	}
	
	if (PerfectDodgeEndTag != FGameplayTag::EmptyTag)
	{
		UAbilityTask_WaitGameplayEvent* PerfectDodgeEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PerfectDodgeEndTag);
		PerfectDodgeEndTask->EventReceived.AddDynamic(this, &ThisClass::HandlePerfectDodgeEnded);
		PerfectDodgeEndTask->ReadyForActivation();
	}
}

void URsGameplayAbility_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (PerfectDodgeSelfEffectHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(PerfectDodgeSelfEffectHandle);
	}
}

void URsGameplayAbility_Dodge::PlayDashOrBackstepMontage()
{
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
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void URsGameplayAbility_Dodge::HandleMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void URsGameplayAbility_Dodge::HandleMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URsGameplayAbility_Dodge::HandlePerfectDodgeStarted(FGameplayEventData Data)
{
	if (PerfectDodgeEffectToSelf)
	{
		const UGameplayEffect* SelfEffect = PerfectDodgeEffectToSelf->GetDefaultObject<UGameplayEffect>();
		PerfectDodgeSelfEffectHandle = ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SelfEffect, GetAbilityLevel());
	}
	
	WaitDamageEffectTask = UAbilityTask_WaitGameplayEffectApplied_Self::WaitGameplayEffectAppliedToSelf(this, FGameplayTargetDataFilterHandle(), FGameplayTagRequirements(), FGameplayTagRequirements());
	WaitDamageEffectTask->OnApplied.AddDynamic(this, &ThisClass::HandleDamageEffectApplied);
	WaitDamageEffectTask->ReadyForActivation();
}

void URsGameplayAbility_Dodge::HandlePerfectDodgeEnded(FGameplayEventData Data)
{
	if (PerfectDodgeSelfEffectHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(PerfectDodgeSelfEffectHandle);
	}

	if (WaitDamageEffectTask)
	{
		WaitDamageEffectTask->EndTask();
		WaitDamageEffectTask = nullptr;
	}
}

void URsGameplayAbility_Dodge::HandleDamageEffectApplied(AActor* Source, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer DamageTags;
	SpecHandle.Data->GetAllAssetTags(DamageTags);
	if (DamageTags.HasAll(DamageTagsCannotDodge))
	{
		return;
	}
	else if (DamageTags.HasAll(DamageTagsCanDodge))
	{
		K2_OnPerfectDodgeSuccess(Source, SpecHandle, ActiveHandle);
	}
}