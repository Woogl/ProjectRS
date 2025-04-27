// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyState_AbilityBase.h"
#include "Rs/Camera/RsCameraTypes.h"
#include "RsAnimNotifyState_CameraMode.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_CameraMode : public URsAnimNotifyState_AbilityBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	ERsCameraRig CameraRig = ERsCameraRig::ThirdPerson;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	ERsCharacterFacingMode FacingMode = ERsCharacterFacingMode::MovementDirection;
	
protected:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;
};
