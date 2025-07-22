// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyState_Targeting.h"
#include "RsAnimNotifyState_TurnAround.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_TurnAround : public URsAnimNotifyState_Targeting
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_TurnAround();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTurnAroundSpeed = 300.f;
	
protected:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(Transient)
	bool bTurnComplete = false;
};
