// Copyright 2025 Team BH.


#include "RsConditionTask.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitEnhancedInput.h"

void URsConditionTaskBase::Activate(UObject* ContextObject)
{
	if (const AActor* Owner = Cast<AActor>(ContextObject))
	{
		OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
		if (const UAbilitySystemComponent* ASC = OwnerASC.Get())
		{
			CurrentAbility = ASC->GetAnimatingAbility();
		}
	}
}

void URsConditionTaskBase::Deactivate(UObject* ContextObject)
{
}

void URsConditionTask_WaitEffect::Activate(UObject* ContextObject)
{
	Super::Activate(ContextObject);
	
}

void URsConditionTask_WaitEffect::Deactivate(UObject* ContextObject)
{
	Super::Deactivate(ContextObject);
	
}

void URsConditionTask_WaitInput::Activate(UObject* ContextObject)
{
	Super::Activate(ContextObject);
	
	if (InputAction && CurrentAbility.Get())
	{
		WaitInputTask = URsAbilityTask_WaitEnhancedInput::WaitEnhancedInput(CurrentAbility.Get(), NAME_None, InputAction, true);
		WaitInputTask->InputEventReceived.AddDynamic(this, &ThisClass::HandleInputReceived);
		WaitInputTask->ReadyForActivation();
	}
}

void URsConditionTask_WaitInput::Deactivate(UObject* ContextObject)
{
	Super::Deactivate(ContextObject);
	
	if (WaitInputTask)
	{
		WaitInputTask->EndTask();
	}
}

void URsConditionTask_WaitInput::HandleInputReceived(const FInputActionValue& InputActionValue)
{
	OnTriggered.Broadcast();
}
