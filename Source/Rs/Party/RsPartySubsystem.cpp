// Copyright 2024 Team BH.


#include "RsPartySubsystem.h"

#include "Rs/Character/RsPlayerCharacter.h"

URsPartySubsystem* URsPartySubsystem::Get(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer)
	{
		return LocalPlayer->GetSubsystem<URsPartySubsystem>();
	}
	return nullptr;
}

void URsPartySubsystem::JoinPartyMember(TSubclassOf<ARsPlayerCharacter> NewMember)
{
	PartyMemberClasses.AddUnique(NewMember);
}

void URsPartySubsystem::LeavePartyMember(TSubclassOf<ARsPlayerCharacter> MemberToRemove)
{
	PartyMemberClasses.Remove(MemberToRemove);
}

TArray<TSubclassOf<ARsPlayerCharacter>> URsPartySubsystem::GetPartyMemberClasses() const
{
	return PartyMemberClasses;
}