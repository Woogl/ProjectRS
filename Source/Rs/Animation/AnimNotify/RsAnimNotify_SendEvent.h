// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotify_SendEvent.generated.h"

UENUM()
enum class ERsEventRecipient : uint8
{
	Source,
	Target
};

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_SendEvent : public URsAnimNotifyBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta=(Categories="AnimNotify"))
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere)
	ERsEventRecipient TargetType;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "TargetType == ERsEventRecipient::Target", EditConditionHides))
	FRsTargetingParams TargetingParams;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
