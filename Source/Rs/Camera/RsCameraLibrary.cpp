// Copyright 2024 Team BH.


#include "RsCameraLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Rs/RsLogChannels.h"
#include "Rs/Player/RsPlayerController.h"

UGameplayCameraComponent* URsCameraLibrary::GetPlayerCameraComponent(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(PlayerController))
		{
			return RsPlayerController->GetGameplayCameraComponent();
		}
	}

	return nullptr;
}

void URsCameraLibrary::SwitchCameraMode(const UObject* WorldContextObject, ERsCameraMode NewCameraMode)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(PlayerController))
		{
			RsPlayerController->CameraMode = NewCameraMode;
		}
	}
}
