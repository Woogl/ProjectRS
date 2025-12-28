// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotifyState_ComboAbility.generated.h"

struct FInputActionValue;
class UAbilitySystemComponent;
class URsAbilityTask_WaitEnhancedInput;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_ComboAbility : public URsAnimNotifyStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="AnimNotify", meta=(Categories="Ability"))
	FGameplayTagContainer AbilityTags;

	UPROPERTY(EditAnywhere, Category="AnimNotify")
	bool bCancelCurrentAbility = false;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	UFUNCTION()
	void HandleInputPressed(const FInputActionValue& Value, const APawn* Pawn);
	
private:
	TWeakObjectPtr<URsAbilityTask_WaitEnhancedInput> Task;
};
