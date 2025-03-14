// Copyright 2024 Team BH.


#include "RsAnimNotifyState_ComboAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitEnhancedInput.h"

void URsAnimNotifyState_ComboAbility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
		if (OwnerASC.IsValid())
		{
			CurrentAbility = OwnerASC->GetAnimatingAbility();
			if (CurrentAbility.IsValid())
			{
				WaitInputTask = URsAbilityTask_WaitEnhancedInput::WaitEnhancedInput(CurrentAbility.Get(), NAME_None, InputAction, bShouldTriggerOnce);
				WaitInputTask->InputEventReceived.AddDynamic(this, &ThisClass::HandleInputAction);
				WaitInputTask->ReadyForActivation();
			}
		}
	}
}

void URsAnimNotifyState_ComboAbility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (WaitInputTask.IsValid())
	{
		WaitInputTask->EndTask();
	}
}

void URsAnimNotifyState_ComboAbility::HandleInputAction(const FInputActionValue& Value)
{
	if (OwnerASC.IsValid() && AbilityTag.IsValid())
	{
		CurrentAbility.Get()->K2_CancelAbility();
		OwnerASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
	}
}
