// Copyright 2024 Team BH.


#include "RsPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "Rs/RsLogChannels.h"

ARsPlayerState::ARsPlayerState()
{
	IConsoleCommand* SetUserIdConsoleCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("rs.Account.SetUserID"),
		TEXT("Set local player's User ID.  ex) rs.Account.SetUserID [num]"),
		FConsoleCommandWithArgsDelegate::CreateUObject(this, &ThisClass::SetUserId_Cheat),
		ECVF_Cheat
	);
}

void ARsPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ARsPlayerState, UserId, COND_None, REPNOTIFY_Always);
}

uint32 ARsPlayerState::GetUserId() const
{
	return UserId;
}

void ARsPlayerState::SetUserId(uint32 NewUserId)
{
	UserId = NewUserId;
}

void ARsPlayerState::OnRep_UserId()
{
}

void ARsPlayerState::SetUserId_Cheat(const TArray<FString>& Args)
{
	if (Args.Num() == 0)
	{
		return;
	}

	int32 NewUserId = FCString::Atoi(*Args[0]);
	SetUserId(NewUserId);
	
	UE_LOG(RsLog, Log, TEXT("New User ID : [%d]"), NewUserId);
}

