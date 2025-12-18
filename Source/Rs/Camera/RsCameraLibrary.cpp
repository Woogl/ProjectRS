// Copyright 2024 Team BH.


#include "RsCameraLibrary.h"

#include "GameFramework/Character.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsPlayerCharacter.h"

UGameplayCameraComponent* URsCameraLibrary::GetPlayerGameplayCameraComponent(const UObject* WorldContextObject, int32 PlayerIndex)
{
	if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0))
	{
		return PlayerCharacter->FindComponentByClass<UGameplayCameraComponent>();
	}
	return nullptr;
}

void URsCameraLibrary::SwitchPlayerCameraRig(const UObject* WorldContextObject, ERsCameraRig CameraRig)
{
	if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0))
	{
		if (ARsPlayerCharacter* RsPlayerCharacter = Cast<ARsPlayerCharacter>(PlayerCharacter))
		{
			RsPlayerCharacter->SetCameraRig(CameraRig);
		}
	}
}