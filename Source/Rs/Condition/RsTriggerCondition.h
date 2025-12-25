// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "UObject/Object.h"
#include "RsTriggerCondition.generated.h"

struct FInputActionValue;
class URsAbilityTask_WaitEnhancedInput;
class UInputAction;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRsOnConditionTriggered);

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class RS_API URsTriggerConditionBase : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FRsOnConditionTriggered OnTriggered;
	
	UFUNCTION(BlueprintCallable)
	virtual void Initialize(UObject* ContextObject);
	
	UFUNCTION(BlueprintCallable)
	virtual void Deinitialize(UObject* ContextObject);
	
protected:
	TWeakObjectPtr<UAbilitySystemComponent> OwnerASC;
	TWeakObjectPtr<UGameplayAbility> CurrentAbility;
};

UCLASS()
class RS_API URsConditionTask_WaitEffect : public URsTriggerConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FGameplayEffectQuery EffectQuery;
	
	UPROPERTY(EditAnywhere)
	bool bIncludeBlockedImmunity = false;
	
	virtual void Initialize(UObject* ContextObject) override;
	virtual void Deinitialize(UObject* ContextObject) override;
};

UCLASS()
class RS_API URsConditionTask_WaitInput : public URsTriggerConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputAction;
	
	virtual void Initialize(UObject* ContextObject) override;
	virtual void Deinitialize(UObject* ContextObject) override;
	
private:
	UFUNCTION()
	void HandleInputReceived(const FInputActionValue& InputActionValue);
	
	UPROPERTY(Transient)
	TObjectPtr<URsAbilityTask_WaitEnhancedInput> WaitInputTask;
};