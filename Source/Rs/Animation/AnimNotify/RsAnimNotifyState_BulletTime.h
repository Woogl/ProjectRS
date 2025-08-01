// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RsAnimNotifyState_BulletTime.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_BulletTime : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeDilation = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendTime = 0.f;
};
