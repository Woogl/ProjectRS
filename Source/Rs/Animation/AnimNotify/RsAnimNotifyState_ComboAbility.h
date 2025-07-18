// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyState_AbilityBase.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "RsAnimNotifyState_ComboAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;
class URsAbilityTask_WaitEnhancedInput;
struct FInputActionValue;
class UInputAction;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_ComboAbility : public URsAnimNotifyState_AbilityBase
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_ComboAbility();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Condition")
	FGameplayTag WaitEventTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="Ability"), Category="AnimNotify")
	FGameplayTag AbilityTag;

private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	void HandleGameplayEvent(FGameplayEventData Payload);
	
	UFUNCTION()
	void HandleInputAction(const FInputActionValue& Value);

	TWeakObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEventTask;
	TWeakObjectPtr<URsAbilityTask_WaitEnhancedInput> WaitInputTask;
};
