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

	URsCameraLibrary::SwitchCameraMode(GetOwnerActor(), CameraRig, FacingMode);
}

void URsAbilityTask_CameraMode::OnDestroy(bool AbilityIsEnding)
{
	URsCameraLibrary::SwitchCameraMode(GetOwnerActor(), ERsCameraRig::ThirdPerson, ERsCharacterFacingMode::MovementDirection);
	
	Super::OnDestroy(AbilityIsEnding);
}
