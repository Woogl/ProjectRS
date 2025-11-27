// Copyright 2024 Team BH.


#include "RsCameraLibrary.h"

#include "GameFramework/Character.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/GameplayStatics.h"

// void URsCameraLibrary::SetCameraRig(const UObject* WorldContextObject, ERsCameraRig CameraRig)
// {
// 	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
// 	{
// 		if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(PlayerController))
// 		{
// 			RsPlayerController->CameraRig = CameraRig;
// 		}
// 	}
// }

UGameplayCameraComponent* URsCameraLibrary::GetPlayerGameplayCameraComponent(const UObject* WorldContextObject)
{
	if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0))
	{
		return PlayerCharacter->FindComponentByClass<UGameplayCameraComponent>();
	}
	return nullptr;
}
