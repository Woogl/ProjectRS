// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RsAbilityTask_WaitEnhancedInput.generated.h"

class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnhancedInputEventDelegate, const FInputActionValue&, Value);

/**
 * 
 */
UCLASS()
class RS_API URsAbilityTask_WaitEnhancedInput : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FEnhancedInputEventDelegate InputEventReceived;
	
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"), Category = "Ability Tasks")
	static URsAbilityTask_WaitEnhancedInput* WaitEnhancedInput(UGameplayAbility* OwningAbility, const FName TaskInstanceName, UInputAction* InputAction, bool bShouldOnlyTriggerOnce = true);

private:
	TWeakObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = nullptr;
	TWeakObjectPtr<UInputAction> InputAction = nullptr;
	bool bTriggerOnce = true;
	bool bHasBeenTriggered = false;

	virtual void Activate() override;
	void HandleInputAction(const FInputActionValue& Value);
	virtual void OnDestroy(const bool bInOwnerFinished) override;
};
