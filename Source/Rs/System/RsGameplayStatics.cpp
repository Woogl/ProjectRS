// Copyright 2025 Team BH.


#include "RsGameplayStatics.h"

#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Player/RsPlayerController.h"
#include "Rs/Player/RsPlayerState.h"

ARsPlayerState* URsGameplayStatics::GetRsPlayerState(const UObject* WorldContextObject, int32 PlayerIndex)
{
	if (APlayerState* PlayerState = GetPlayerState(WorldContextObject, PlayerIndex))
	{
		return Cast<ARsPlayerState>(PlayerState);
	}
	return nullptr;
}

ARsPlayerController* URsGameplayStatics::GetRsPlayerController(const UObject* WorldContextObject, int32 PlayerIndex)
{
	if (APlayerController* PlayerController = GetPlayerController(WorldContextObject, PlayerIndex))
	{
		return Cast<ARsPlayerController>(PlayerController);
	}
	return nullptr;
}

ARsPlayerCharacter* URsGameplayStatics::GetRsPlayerCharacter(const UObject* WorldContextObject, int32 PlayerIndex)
{
	if (ACharacter* PlayerCharacter = GetPlayerCharacter(WorldContextObject, PlayerIndex))
	{
		return Cast<ARsPlayerCharacter>(PlayerCharacter);
	}
	return nullptr;
}
