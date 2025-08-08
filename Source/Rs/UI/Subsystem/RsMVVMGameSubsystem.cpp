// Copyright 2025 Team BH.


#include "RsMVVMGameSubsystem.h"

#include "Kismet/GameplayStatics.h"

URsMVVMGameSubsystem* URsMVVMGameSubsystem::Get(UObject* WorldContext)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContext))
	{
		return GameInstance->GetSubsystem<URsMVVMGameSubsystem>();
	}
	return nullptr;
}
