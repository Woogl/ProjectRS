// Copyright 2024 Team BH.


#include "RsPartyViewModel.h"

#include "RsPlayerCharacterViewModel.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/Player/RsPlayerController.h"

URsPartyViewModel* URsPartyViewModel::CreateRsPartyViewModel(URsPartyComponent* PartyComponent)
{
	URsPartyViewModel* ViewModel = NewObject<URsPartyViewModel>(PartyComponent);
	ViewModel->Initialize();
	return ViewModel;
}

void URsPartyViewModel::Initialize()
{
	Super::Initialize();

	if (URsPartyComponent* PartyComponent = Cast<URsPartyComponent>(GetOuter()))
	{
		PartyComponent->OnAddPartyMember.AddUObject(this, &ThisClass::HandleAddPartyMember);
		PartyComponent->OnRemovePartyMember.AddUObject(this, &ThisClass::HandleRemovePartyMember);

		TArray<ARsPlayerCharacter*> PartyMembers = PartyComponent->GetPartyMembers();
		for (int32 i = 0; i < PartyMembers.Num() -1; ++i)
		{
			URsPlayerCharacterViewModel* CharacterViewModel = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(PartyMembers[i]);
			if (i == 0)
			{
				UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_0, CharacterViewModel);
			}
			else if (i == 1)
			{
				UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_1, CharacterViewModel);
			}
			else if (i == 2)
			{
				UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_2, CharacterViewModel);
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
	URsPlayerCharacterViewModel* NewCharacterViewModel = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(PartyMember);
	if (MemberIndex == 0)
	{
		UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_0, NewCharacterViewModel);
	}
	else if (MemberIndex == 1)
	{
		UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_1, NewCharacterViewModel);
	}
	else if (MemberIndex == 2)
	{
		UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_2, NewCharacterViewModel);
	}
}

void URsPartyViewModel::HandleRemovePartyMember(ARsPlayerCharacter* PartyMember, int32 MemberIndex)
{
	if (MemberIndex == 0)
	{
		UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_0, nullptr);
	}
	else if (MemberIndex == 1)
	{
		UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_1, nullptr);
	}
	else if (MemberIndex == 2)
	{
		UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_2, nullptr);
	}
}
