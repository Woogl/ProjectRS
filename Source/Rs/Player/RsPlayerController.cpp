// Copyright 2024 Team BH.


#include "RsPlayerController.h"

#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyComponent.h"

ARsPlayerController::ARsPlayerController()
{
	PartyComponent = CreateDefaultSubobject<URsPartyComponent>(TEXT("PartyComponent"));
}

void ARsPlayerController::SwitchPartyMember(int32 MemberIndex)
{
	if (ARsPlayerCharacter* NewPartyMember = PartyComponent->GetPartyMember(MemberIndex))
	{
		if (GetPawn() != NewPartyMember)
		{
			Possess(NewPartyMember);
		}
	}
}

URsPartyComponent* ARsPlayerController::GetPartyComponent() const
{
	return PartyComponent;
}
