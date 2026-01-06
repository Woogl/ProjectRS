// Copyright 2024 Team BH.


#include "RsUIManagerSubsystem.h"

#include "Kismet/GameplayStatics.h"

URsUIManagerSubsystem* URsUIManagerSubsystem::Get(const UObject* WorldContextObject)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return GameInstance->GetSubsystem<URsUIManagerSubsystem>();
	}
	return nullptr;
}
