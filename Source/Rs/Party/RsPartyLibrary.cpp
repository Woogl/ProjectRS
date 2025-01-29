// Copyright 2024 Team BH.


#include "RsPartyLibrary.h"

#include "RsPartyComponent.h"
#include "RsPartySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Player/RsPlayerController.h"

void URsPartyLibrary::SwitchPartyMember(UObject* WorldContextObject, int32 NewMemberIndex)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		RsPlayerController->GetPartyComponent()->SwitchPartyMember(RsPlayerController, NewMemberIndex);
	}
}

void URsPartyLibrary::AddPartyMember(ARsPlayerCharacter* NewMember)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(NewMember, 0)))
	{
		RsPlayerController->GetPartyComponent()->AddPartyMember(NewMember);
	}
}

void URsPartyLibrary::RemovePartyMember(ARsPlayerCharacter* MemberToRemove)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(MemberToRemove, 0)))
	{
		RsPlayerController->GetPartyComponent()->RemovePartyMember(MemberToRemove);
	}
}

void URsPartyLibrary::AddPartyMemberClass(ULocalPlayer* LocalPlayer, TSubclassOf<ARsPlayerCharacter> NewMemberClass)
{
	if (URsPartySubsystem* PartySubsystem = URsPartySubsystem::Get(LocalPlayer))
	{
		PartySubsystem->AddPartyMemberClass(NewMemberClass);
	}
}

void URsPartyLibrary::RemovePartyMemberClass(ULocalPlayer* LocalPlayer, TSubclassOf<ARsPlayerCharacter> MemberClassToRemove)
{
	if (URsPartySubsystem* PartySubsystem = URsPartySubsystem::Get(LocalPlayer))
	{
		PartySubsystem->RemovePartyMemberClass(MemberClassToRemove);
	}
}
