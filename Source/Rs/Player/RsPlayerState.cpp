// Copyright 2024 Team BH.


#include "RsPlayerState.h"

#include "Net/UnrealNetwork.h"

ARsPlayerState::ARsPlayerState()
{
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
