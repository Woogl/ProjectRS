// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyBase.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_TurnTo.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_TurnTo : public URsAnimNotifyStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FRsTargetingParams TargetingParams;
	
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
};
