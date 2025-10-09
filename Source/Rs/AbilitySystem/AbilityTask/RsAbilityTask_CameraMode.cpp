// Copyright 2024 Team BH.


#include "RsAbilityTask_CameraMode.h"

#include "Net/UnrealNetwork.h"
#include "Rs/Camera/RsCameraLibrary.h"

URsAbilityTask_CameraMode* URsAbilityTask_CameraMode::SwitchCameraMode(UGameplayAbility* OwningAbility, ERsCameraRig CameraRig, ERsCharacterFacingMode FacingMode)
{
	URsAbilityTask_CameraMode* Task = NewAbilityTask<URsAbilityTask_CameraMode>(OwningAbility);
	Task->CameraRig = CameraRig;
	Task->FacingMode = FacingMode;
	return Task;
}

void URsAbilityTask_CameraMode::Activate()
{
	Super::Activate();

	URsCameraLibrary::SetCameraRig(GetOwnerActor(), CameraRig);
	URsCameraLibrary::SetCharacterFacingMode(GetOwnerActor(), FacingMode);
}

void URsAbilityTask_CameraMode::OnDestroy(bool AbilityIsEnding)
{
	URsCameraLibrary::SetCameraRig(GetOwnerActor(), ERsCameraRig::FreeCam);
	URsCameraLibrary::SetCharacterFacingMode(GetOwnerActor(), ERsCharacterFacingMode::MovementDirection);
	
	Super::OnDestroy(AbilityIsEnding);
}
