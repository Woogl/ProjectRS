// Copyright 2024 Team BH.


#include "RsPartyLibrary.h"

#include "RsPartyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Player/RsPlayerController.h"

void URsPartyLibrary::SwitchPartyMember(UObject* WorldContextObject, int32 NewMemberIndex)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		RsPlayerController->SwitchPartyMember(NewMemberIndex);
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
