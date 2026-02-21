// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotify_SendEvent.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_SendEvent : public URsAnimNotifyBase
{
	GENERATED_BODY()
	
public:
	URsAnimNotify_SendEvent();
	
	UPROPERTY(EditAnywhere, meta=(Categories="AnimNotify"))
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere)
	ERsTargetType TargetType;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "TargetType == ERsTargetType::Target", EditConditionHides))
	FRsTargetingParams TargetingParams;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
