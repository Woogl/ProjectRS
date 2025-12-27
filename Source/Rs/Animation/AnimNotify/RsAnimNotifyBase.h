// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RsAnimNotifyBase.generated.h"

class URsTriggerConditionBase;
class UGameplayAbility;
class UAbilitySystemComponent;
class URsConditionBase;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyBase : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Instanced, EditAnywhere)
	TObjectPtr<URsConditionBase> Condition;
	
	UFUNCTION(BlueprintCallable)
	bool PassCondition(AActor* Owner) const;
};

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyStateBase : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Instanced, EditAnywhere)
	TObjectPtr<URsConditionBase> Condition;
	
	UFUNCTION(BlueprintCallable)
	bool PassCondition(AActor* Owner) const;
};