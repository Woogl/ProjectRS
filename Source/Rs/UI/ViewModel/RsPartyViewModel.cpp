// Copyright 2024 Team BH.


#include "RsPartyViewModel.h"

#include "RsPlayerCharacterViewModel.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/Player/RsPlayerController.h"

URsPartyViewModel* URsPartyViewModel::CreateRsPartyViewModel(APlayerController* PlayerController)
{
	URsPartyViewModel* ViewModel = NewObject<URsPartyViewModel>(PlayerController);
	ViewModel->Initialize();
	return ViewModel;
}

void URsPartyViewModel::Initialize()
{
	Super::Initialize();

	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(GetOuter()))
	{
		if (URsPartyComponent* PartyComponent = RsPlayerController->GetPartyComponent())
		{
			PartyComponent->OnAddPartyMember.AddUObject(this, &ThisClass::HandleAddPartyMember);
			PartyComponent->OnRemovePartyMember.AddUObject(this, &ThisClass::HandleRemovePartyMember);

			TArray<ARsPlayerCharacter*> PartyMembers = PartyComponent->GetPartyMembers();
			for (int32 i = 0; i < PartyMembers.Num() -1; ++i)
			{
				if (i == 0)
				{
					CharacterViewModel_0 = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(PartyMembers[i]);
				}
				else if (i == 1)
				{
					CharacterViewModel_1 = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(PartyMembers[i]);
				}
				else if (i == 2)
				{
					CharacterViewModel_2 = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(PartyMembers[i]);
				}
			}
		}
	}
}

void URsPartyViewModel::Deinitialize()
{
	Super::Deinitialize();

	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(GetOuter()))
	{
		if (URsPartyComponent* PartyComponent = RsPlayerController->GetPartyComponent())
		{
			PartyComponent->OnAddPartyMember.RemoveAll(this);
			PartyComponent->OnRemovePartyMember.RemoveAll(this);
		}
	}
}

void URsPartyViewModel::HandleAddPartyMember(ARsPlayerCharacter* PartyMember, int32 MemberIndex)
{
	if (MemberIndex == 0)
	{
		CharacterViewModel_0 = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(PartyMember);
	}
	else if (MemberIndex == 1)
	{
		CharacterViewModel_1 = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(PartyMember);
	}
	else if (MemberIndex == 2)
	{
		CharacterViewModel_2 = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(PartyMember);
	}
}

void URsPartyViewModel::HandleRemovePartyMember(ARsPlayerCharacter* PartyMember, int32 MemberIndex)
{
	if (MemberIndex == 0)
	{
		CharacterViewModel_0 = nullptr;
	}
	else if (MemberIndex == 1)
	{
		CharacterViewModel_1 = nullptr;
	}
	else if (MemberIndex == 2)
	{
		CharacterViewModel_2 = nullptr;
	}
}
