// Copyright 2024 Team BH.


#include "RsCameraLibrary.h"

#include "GameFramework/GameplayCameraComponent.h"
#include "GameFramework/BlueprintCameraVariableTable.h"
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

void URsCameraLibrary::SwitchCameraMode(const UObject* WorldContextObject, ERsCameraMode NewCameraMode)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(PlayerController))
		{
			RsPlayerController->CameraMode = NewCameraMode;

			switch (NewCameraMode)
			{
			case ERsCameraMode::ShoulderView:
				SwitchCharacterRotationMode(RsPlayerController->GetCharacter(), ERsCharacterRotationMode::CameraDirection);
				break;

			default:
				SwitchCharacterRotationMode(RsPlayerController->GetCharacter(), ERsCharacterRotationMode::MovementDirection);
				break;
			}
		}
	}
}

void URsCameraLibrary::SwitchCharacterRotationMode(ACharacter* Character, ERsCharacterRotationMode Mode)
{
	if (Character == nullptr)
	{
		return;
	}
	
	switch (Mode)
	{
	case ERsCharacterRotationMode::MovementDirection:
		Character->bUseControllerRotationYaw = false;
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		break;
		
	case ERsCharacterRotationMode::CameraDirection:
		Character->bUseControllerRotationYaw = true;
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		break;
		
	default:
		break;
	}
}
