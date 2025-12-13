// Copyright 2025 Team BH.


#include "RsCheatManager.h"

#include "RsPlayerState.h"

DEFINE_LOG_CATEGORY(LogRsCheat);

void URsCheatManager::InitCheatManager()
{
	Super::InitCheatManager();
}

ARsPlayerState* URsCheatManager::GetRsPlayerState() const
{
	if (APlayerController* PlayerController = GetPlayerController())
	{
		return PlayerController->GetPlayerState<ARsPlayerState>();
	}
	return nullptr;
}

void URsCheatManager::SetUserId(int32 NewId)
{
	if (ARsPlayerState* RsPlayerState = GetRsPlayerState())
	{
		RsPlayerState->SetUserId(NewId);
		
		UE_LOG(LogRsCheat, Log, TEXT("[%s]'s User ID : [%d]"), *RsPlayerState->GetName(), RsPlayerState->GetUserId());
	}
}
