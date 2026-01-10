// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotifyState_ComboWindow.generated.h"

class URsAbilityTask_WaitEnhancedInput;
struct FInputActionValue;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_ComboWindow : public URsAnimNotifyStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="AnimNotify", meta=(Categories="Ability"))
	TArray<FGameplayTagContainer> AbilityTagsList;

	UPROPERTY(EditAnywhere, Category="AnimNotify")
	bool bCancelCurrentAbility = false;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	// UFUNCTION()
	void HandleInputTriggered(const FInputActionValue& Value, USkeletalMeshComponent* MeshComp, const FGameplayTagContainer& AbilityTags);
	
	struct FComboWindowRuntimeData
	{
		TArray<TWeakObjectPtr<URsAbilityTask_WaitEnhancedInput>> Tasks;
	};
	
	TMap<TWeakObjectPtr<USkeletalMeshComponent>, FComboWindowRuntimeData> RuntimeDataMap;
};
