// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RsAnimNotifyState_ComboAbility.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;
class URsAbilityTask_WaitEnhancedInput;
struct FInputActionValue;
class UInputAction;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_ComboAbility : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="Ability"), Category="AnimNotify")
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	bool bShouldTriggerOnce = true;

private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	void HandleInputAction(const FInputActionValue& Value);

	TWeakObjectPtr<UAbilitySystemComponent> OwnerASC;
	TWeakObjectPtr<UGameplayAbility> CurrentAbility;
	TWeakObjectPtr<URsAbilityTask_WaitEnhancedInput> WaitInputTask;
};
