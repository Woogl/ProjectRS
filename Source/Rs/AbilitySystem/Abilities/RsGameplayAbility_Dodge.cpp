// Copyright 2024 Team BH.


#include "RsGameplayAbility_Dodge.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
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
		URsAbilityTask_TurnToLocation* TurnTask = URsAbilityTask_TurnToLocation::TurnToLocation(this, TargetLocation, TurnAroundSpeed, TurnAroundMaxDuration);
		// Reserve Dash montage
		TurnTask->OnFinish.AddDynamic(this, &ThisClass::PlayDashOrBackstepMontage);
		TurnTask->ReadyForActivation();
	}
	else
	{
		// Play Backstep montage
		PlayDashOrBackstepMontage();
	}
}

void URsGameplayAbility_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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
