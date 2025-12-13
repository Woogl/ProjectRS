// Copyright 2024 Team BH.


#include "RsAnimNotifyState_ComboAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitEnhancedInput.h"

URsAnimNotifyState_ComboAbility::URsAnimNotifyState_ComboAbility()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotifyState_ComboAbility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (CurrentAbility.IsValid())
	{
		if (WaitEventTag.IsValid())
		{
			WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(CurrentAbility.Get(), WaitEventTag);
			WaitEventTask->EventReceived.AddDynamic(this, &ThisClass::HandleGameplayEvent);
			WaitEventTask->ReadyForActivation();
		}
		else
		{
			WaitInputTask = URsAbilityTask_WaitEnhancedInput::WaitEnhancedInput(CurrentAbility.Get(), NAME_None, InputAction);
			WaitInputTask->InputEventReceived.AddDynamic(this, &ThisClass::HandleInputAction);
			WaitInputTask->ReadyForActivation();
		}
	}
}

void URsAnimNotifyState_ComboAbility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (WaitEventTask.IsValid())
	{
		WaitEventTask->EndTask();
	}
	
	if (WaitInputTask.IsValid())
	{
		WaitInputTask->EndTask();
	}
}

void URsAnimNotifyState_ComboAbility::HandleGameplayEvent(FGameplayEventData Payload)
{
	WaitInputTask = URsAbilityTask_WaitEnhancedInput::WaitEnhancedInput(CurrentAbility.Get(), NAME_None, InputAction);
	WaitInputTask->InputEventReceived.AddDynamic(this, &ThisClass::HandleInputAction);
	WaitInputTask->ReadyForActivation();
}

void URsAnimNotifyState_ComboAbility::HandleInputAction(const FInputActionValue& Value)
{
	if (OwnerASC.IsValid() && AbilityTag.IsValid() && CurrentAbility.IsValid())
	{
		if (bCancelCurrentAbility)
		{
			CurrentAbility.Get()->K2_CancelAbility();
		}
		OwnerASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
	}
}
