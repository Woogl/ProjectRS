// Copyright 2024 Team BH.


#include "RsPartyComponent.h"

#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Player/RsPlayerController.h"

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

void URsPartyComponent::SwitchPartyMember(ARsPlayerController* PlayerController, int32 MemberIndex)
{
	if (ARsPlayerCharacter* NewPartyMember = GetPartyMember(MemberIndex))
	{
		if (PlayerController->GetPawn() != NewPartyMember)
		{
			PlayerController->Possess(NewPartyMember);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RsPartyComponent::SwitchPartyMember: Can't switch to same character"));
		}
	}
}
