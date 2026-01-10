// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotifyState_ComboAbility.generated.h"

class UInputAction;
struct FInputActionValue;
class URsAbilityTask_WaitEnhancedInput;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_ComboAbility : public URsAnimNotifyStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputAction;
	
	UPROPERTY(EditAnywhere, meta=(Categories="Ability"))
	FGameplayTagContainer AbilityTags;

	UPROPERTY(EditAnywhere)
	bool bCancelCurrentAbility = false;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	void HandleInputPressed(const FInputActionValue& Value, USkeletalMeshComponent* MeshComp, const FGameplayTagContainer& InAbilityTags);
	
private:
	TWeakObjectPtr<URsAbilityTask_WaitEnhancedInput> Task;
};
