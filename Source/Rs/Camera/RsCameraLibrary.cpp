// Copyright 2024 Team BH.


#include "RsCameraLibrary.h"

#include "Core/CameraVariableAssets.h"
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

void URsCameraLibrary::SwitchCameraRig(const UObject* WorldContextObject, ERsCameraRig CameraRig)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(PlayerController))
		{
			RsPlayerController->CameraRig = CameraRig;
		}
	}
}

void URsCameraLibrary::SwitchCharacterFacingMode(const UObject* WorldContextObject, ERsCharacterFacingMode FacingMode)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ACharacter* Character = PlayerController->GetCharacter())
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

void URsCameraLibrary::SwitchCameraMode(const UObject* WorldContextObject, ERsCameraRig CameraRig, ERsCharacterFacingMode FacingMode)
{
	SwitchCameraRig(WorldContextObject, CameraRig);
	SwitchCharacterFacingMode(WorldContextObject, FacingMode);
}

void URsCameraLibrary::SetCameraVariableAsVector(const UObject* WorldContextObject, UVector3dCameraVariable* Variable, FVector Value)
{
	if (UGameplayCameraComponent* PlayerCameraComponent = GetPlayerCameraComponent(WorldContextObject))
	{
		PlayerCameraComponent->GetInitialVariableTable().GetVariableTable()->SetValue<FVector>(Variable->GetVariableID(), Value);
	}
}

