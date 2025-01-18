// Copyright 2024 Team BH.


#include "RsPartyComponent.h"

URsPartyComponent::URsPartyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

ARsPlayerCharacter* URsPartyComponent::GetPartyMember(int32 MemberIndex) const
{
	if (PartyMembers.IsValidIndex(MemberIndex))
	{
		return PartyMembers[MemberIndex];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RsPartyComponent::GetPartyMember: Member Not Found"));
		return nullptr;
	}
}

void URsPartyComponent::AddPartyMember(ARsPlayerCharacter* NewMember)
{
	if (!PartyMembers.Contains(NewMember))
	{
		PartyMembers.Add(NewMember);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RsPartyComponent::AddPartyMember: Already added member"));
	}
}

void URsPartyComponent::RemovePartyMember(ARsPlayerCharacter* MemberToRemove)
{
	if (PartyMembers.Contains(MemberToRemove))
	{
		PartyMembers.Remove(MemberToRemove);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RsPartyComponent::RemovePartyMember: Can't found Member"));
	}
}
