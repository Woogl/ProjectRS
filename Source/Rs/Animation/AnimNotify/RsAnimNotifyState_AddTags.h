// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RsAnimNotifyState_AddTags.generated.h"

/**
 * Add GameplayTag on NotifyBegin().
 * Remove GameplayTag on NotifyEnd().
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_AddTags : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	URsAnimNotifyState_AddTags();
	
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Tags;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
