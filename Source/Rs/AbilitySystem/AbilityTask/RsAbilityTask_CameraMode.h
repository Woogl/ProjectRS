// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Rs/Camera/RsCameraTypes.h"
#include "RsAbilityTask_CameraMode.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAbilityTask_CameraMode : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URsAbilityTask_CameraMode* SwitchCameraMode(UGameplayAbility* OwningAbility, ERsCameraRig CameraRig, ERsCharacterFacingMode FacingMode);

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityIsEnding) override;

private:
	ERsCameraRig CameraRig = ERsCameraRig::FreeCam;
	ERsCharacterFacingMode FacingMode = ERsCharacterFacingMode::MovementDirection;
};
