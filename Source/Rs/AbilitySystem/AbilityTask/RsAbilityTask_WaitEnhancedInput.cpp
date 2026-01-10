// Copyright 2024 Team BH.


#include "RsAbilityTask_WaitEnhancedInput.h"

#include "EnhancedInputComponent.h"

URsAbilityTask_WaitEnhancedInput* URsAbilityTask_WaitEnhancedInput::WaitEnhancedInput(UGameplayAbility* OwningAbility, const FName TaskInstanceName, UInputAction* InputAction, bool bShouldOnlyTriggerOnce)
{
	URsAbilityTask_WaitEnhancedInput* AbilityTask = NewAbilityTask<URsAbilityTask_WaitEnhancedInput>(OwningAbility, TaskInstanceName);
	AbilityTask->InputAction = InputAction;
	AbilityTask->bTriggerOnce = bShouldOnlyTriggerOnce;
	return AbilityTask;
}

void URsAbilityTask_WaitEnhancedInput::Activate()
{
	Super::Activate();
	
	if (!AbilitySystemComponent.Get() || !Ability || !InputAction.IsValid())
	{
		return;
	}

	const APawn* const AvatarPawn = Cast<APawn>(Ability->GetAvatarActorFromActorInfo());
	const APlayerController* const PlayerController = AvatarPawn ? Cast<APlayerController>(AvatarPawn->GetController()) : nullptr;
	
	if (!PlayerController)
	{
		return;
	}

	EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
			
	if (IsValid(EnhancedInputComponent.Get()))
	{
		EnhancedInputComponent->BindAction(InputAction.Get(), ETriggerEvent::Triggered, this, &ThisClass::HandleInputAction, AvatarPawn);
	}
}

void URsAbilityTask_WaitEnhancedInput::HandleInputAction(const FInputActionValue& Value, const APawn* Pawn)
{
	if (bTriggerOnce && bHasBeenTriggered)
	{
		return;
	}
	bHasBeenTriggered = true;
	
	InputEventReceived.Broadcast(Value, Pawn);
	InputEventReceivedNative.Broadcast(Value, Pawn);
}

void URsAbilityTask_WaitEnhancedInput::OnDestroy(const bool bInOwnerFinished)
{
	if (IsValid(EnhancedInputComponent.Get()))
	{
		EnhancedInputComponent->ClearBindingsForObject(this);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}
