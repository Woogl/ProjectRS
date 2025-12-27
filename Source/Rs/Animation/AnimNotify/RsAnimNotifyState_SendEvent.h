// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotify_SendEvent.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_SendEvent.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_SendEvent : public URsAnimNotifyStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta=(Categories="AnimNotify"))
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere)
	ERsEventRecipient TargetType;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "TargetType == ERsEventRecipient::Target", EditConditionHides))
	FRsTargetingParams TargetingParams;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
};
