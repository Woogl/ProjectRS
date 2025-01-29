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

void URsPartySubsystem::AddPartyMemberClass(TSubclassOf<ARsPlayerCharacter> NewMember)
{
	PartyMemberClasses.Add(NewMember);
}

void URsPartySubsystem::RemovePartyMemberClass(TSubclassOf<ARsPlayerCharacter> MemberToRemove)
{
	PartyMemberClasses.Remove(MemberToRemove);
}

TArray<ARsPlayerCharacter*> URsPartySubsystem::SpawnPartyMembers(FTransform SpawnTransform)
{
	if (GetWorld() == nullptr)
	{
		return TArray<ARsPlayerCharacter*>();
	}

	TArray<ARsPlayerCharacter*> PartyMembers;
	for (TSubclassOf<ARsPlayerCharacter> Class : PartyMemberClasses)
	{
		ARsPlayerCharacter* SpawnedActor = GetWorld()->SpawnActor<ARsPlayerCharacter>(Class, SpawnTransform);
		PartyMembers.Add(SpawnedActor);
	}
	return PartyMembers;
}
