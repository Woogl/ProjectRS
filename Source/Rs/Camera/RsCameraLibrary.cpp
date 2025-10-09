// Copyright 2024 Team BH.


#include "RsCameraLibrary.h"

#include "CameraAnimationSequence.h"
#include "RsCameraAnimationActor.h"
#include "Animations/CameraAnimationCameraModifier.h"
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

void URsCameraLibrary::SetCameraRig(const UObject* WorldContextObject, ERsCameraRig CameraRig)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(PlayerController))
		{
			RsPlayerController->CameraRig = CameraRig;
		}
	}
}

void URsCameraLibrary::SetCharacterFacingMode(const UObject* WorldContextObject, ERsCharacterFacingMode FacingMode)
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

ARsCameraAnimationActor* URsCameraLibrary::PlayCameraAnimationSequence(ARsPlayerController* PlayerController, UCameraAnimationSequence* Sequence, FCameraAnimationParams Params)
{
	if (!PlayerController || !Sequence)
	{
		return nullptr;
	}
	
	if (UWorld* World = PlayerController->GetWorld())
	{
		ARsCameraAnimationActor* CameraActor = World->SpawnActor<ARsCameraAnimationActor>(ARsCameraAnimationActor::StaticClass());
		CameraActor->PlayCameraAnimation(PlayerController, Sequence, Params);
		return CameraActor;
	}
	return nullptr;
}

void URsCameraLibrary::StopCameraAnimationSequence(ARsPlayerController* PlayerController, UCameraAnimationSequence* Sequence)
{
	if (ARsCameraAnimationActor* CameraActor = PlayerController->CurrentAnimatonCameraActor.Get())
	{
		if (CameraActor->GetSequence() == Sequence)
		{
			CameraActor->ResetCameraAnimation();
		}
	}
}

