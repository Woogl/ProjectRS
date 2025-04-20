// Copyright 2024 Team BH.


#include "RsCameraLibrary.h"

#include "Core/CameraAsset.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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

void URsCameraLibrary::SwitchCameraMode(const UObject* WorldContextObject, ERsCameraRig CameraRig, ERsCharacterFacingMode FacingMode)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(PlayerController))
		{
			RsPlayerController->CameraRig = CameraRig;

			if (ACharacter* Character = RsPlayerController->GetCharacter())
			{
				if (FacingMode == ERsCharacterFacingMode::MovementDirection)
				{
					Character->bUseControllerRotationYaw = false;
					Character->GetCharacterMovement()->bOrientRotationToMovement = true;
				}
				else if (FacingMode == ERsCharacterFacingMode::CameraDirection)
				{
					Character->bUseControllerRotationYaw = true;
					Character->GetCharacterMovement()->bOrientRotationToMovement = false;
				}
			}
		}
	}
}
