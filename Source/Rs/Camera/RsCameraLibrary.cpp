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

ARsCameraAnimationActor* URsCameraLibrary::PlayCameraAnimationSequence(ARsPlayerController* PlayerController, UCameraAnimationSequence* Sequence, FCameraAnimationParams Params)
{
	if (!PlayerController || !Sequence)
	{
		return nullptr;
	}
	
	if (UWorld* World = PlayerController->GetWorld())
	{
		if (ARsCameraAnimationActor* CameraActor = World->SpawnActorDeferred<ARsCameraAnimationActor>(ARsCameraAnimationActor::StaticClass(), FTransform::Identity))
		{
			CameraActor->Sequence = Sequence;
			CameraActor->Params = Params;
			CameraActor->PlayerController = PlayerController;
			CameraActor->OriginalViewTarget = PlayerController->GetViewTarget();
			PlayerController->CurrentAnimatonCameraActor = CameraActor;

			FTransform SpawnTransform = PlayerController->GetPawn()->GetActorTransform();
			FVector SpawnLocation = SpawnTransform.GetLocation() - FVector(0.f, 0.f, PlayerController->GetPawn()->GetDefaultHalfHeight());
			SpawnTransform.SetLocation(SpawnLocation);
			CameraActor->FinishSpawning(SpawnTransform);
			
			return CameraActor;
		}
	}
	return nullptr;
}

void URsCameraLibrary::StopCameraAnimationSequence(ARsPlayerController* PlayerController, UCameraAnimationSequence* Sequence, bool bImmediate)
{
	if (UCameraAnimationCameraModifier* CameraModifier = UCameraAnimationCameraModifier::GetCameraAnimationCameraModifierFromPlayerController(PlayerController))
	{
		CameraModifier->StopAllCameraAnimationsOf(Sequence, bImmediate);
	}
	
	if (ARsCameraAnimationActor* CameraActor = PlayerController->CurrentAnimatonCameraActor.Get())
	{
		if (CameraActor->Sequence == Sequence)
		{
			CameraActor->Destroy();
		}
	}
}

