// Copyright 2024 Team BH.


#include "RsAnimNotifyState_CameraMode.h"

#include "Rs/Camera/RsCameraLibrary.h"

void URsAnimNotifyState_CameraMode::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	URsCameraLibrary::SwitchCameraMode(MeshComp, CameraRig, FacingMode);
}

void URsAnimNotifyState_CameraMode::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	URsCameraLibrary::SwitchCameraMode(MeshComp, ERsCameraRig::ThirdPerson, ERsCharacterFacingMode::MovementDirection);
}
