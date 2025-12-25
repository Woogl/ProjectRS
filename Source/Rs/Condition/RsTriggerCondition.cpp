// Copyright 2025 Team BH.


#include "RsTriggerCondition.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_WaitEnhancedInput.h"

void URsTriggerConditionBase::Initialize(UObject* ContextObject)
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

void URsTriggerConditionBase::Deinitialize(UObject* ContextObject)
{
}

void URsConditionTask_WaitEffect::Initialize(UObject* ContextObject)
{
	Super::Initialize(ContextObject);
	
}

void URsConditionTask_WaitEffect::Deinitialize(UObject* ContextObject)
{
	Super::Deinitialize(ContextObject);
	
}

void URsConditionTask_WaitInput::Initialize(UObject* ContextObject)
{
	Super::Initialize(ContextObject);
	
	if (InputAction && CurrentAbility.Get())
	{
		WaitInputTask = URsAbilityTask_WaitEnhancedInput::WaitEnhancedInput(CurrentAbility.Get(), NAME_None, InputAction, true);
		WaitInputTask->InputEventReceived.AddDynamic(this, &ThisClass::HandleInputReceived);
		WaitInputTask->ReadyForActivation();
	}
}

void URsConditionTask_WaitInput::Deinitialize(UObject* ContextObject)
{
	Super::Deinitialize(ContextObject);
	
	if (WaitInputTask)
	{
		WaitInputTask->EndTask();
	}
}

void URsConditionTask_WaitInput::HandleInputReceived(const FInputActionValue& InputActionValue)
{
	OnTriggered.Broadcast();
}
