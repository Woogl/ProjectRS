// Copyright 2024 Team BH.


#include "RsAnimNotifyState_ComboAbility.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitEnhancedInput.h"

URsAnimNotifyState_ComboAbility::URsAnimNotifyState_ComboAbility()
{
}

void URsAnimNotifyState_ComboAbility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (CurrentAbility)
	{
		WaitInputTask = URsAbilityTask_WaitEnhancedInput::WaitEnhancedInput(CurrentAbility.Get(), NAME_None, InputAction);
		WaitInputTask->InputEventReceived.AddDynamic(this, &ThisClass::HandleInputAction);
		WaitInputTask->ReadyForActivation();
	}
}

void URsAnimNotifyState_ComboAbility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (WaitInputTask)
	{
		WaitInputTask->EndTask();
	}
}

void URsAnimNotifyState_ComboAbility::HandleInputAction(const FInputActionValue& Value)
{
	if (OwnerASC && AbilityTag.IsValid())
	{
		if (bCancelCurrentAbility && CurrentAbility)
		{
			CurrentAbility->K2_CancelAbility();
		}
		OwnerASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
	}
}
