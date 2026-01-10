// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_GrantTags.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_GrantTags : public URsAnimNotifyStateBase
{
	GENERATED_BODY()
	
public:
	URsAnimNotifyState_GrantTags();
	
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Tags;
	
	UPROPERTY(EditAnywhere)
	ERsTargetType TargetType;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "TargetType == ERsTargetType::Target", EditConditionHides))
	FRsTargetingParams TargetingParams;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
