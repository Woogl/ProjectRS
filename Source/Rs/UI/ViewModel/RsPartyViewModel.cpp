// Copyright 2024 Team BH.


#include "RsPartyViewModel.h"

#include "RsPlayerCharacterViewModel.h"
#include "Rs/Party/RsPartyComponent.h"

URsPartyViewModel* URsPartyViewModel::CreateRsPartyViewModel(URsPartyComponent* PartyComponent)
{
	URsPartyViewModel* ViewModel = NewObject<URsPartyViewModel>(PartyComponent);
	ViewModel->Initialize();
	return ViewModel;
}

void URsPartyViewModel::Initialize()
{
	Super::Initialize();

	CachedModel = Cast<URsPartyComponent>(GetOuter());
	if (URsPartyComponent* Model = CachedModel.Get())
	{
		Model->OnAddPartyMember.AddUObject(this, &ThisClass::HandleAddPartyMember);
		Model->OnRemovePartyMember.AddUObject(this, &ThisClass::HandleRemovePartyMember);

		TArray<ARsPlayerCharacter*> PartyMembers = Model->GetPartyMembers();
		for (int32 i = 0; i < PartyMembers.Num(); ++i)
		{
			URsPlayerCharacterViewModel* CharacterViewModel = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(PartyMembers[i]);
			if (i == 0)
			{
				SetPartyMemberViewModel_0(CharacterViewModel);
			}
			else if (i == 1)
			{
				SetPartyMemberViewModel_1(CharacterViewModel);
			}
			else if (i == 2)
			{
				SetPartyMemberViewModel_2(CharacterViewModel);
			}
		}
	}
}

void URsPartyViewModel::Deinitialize()
{
	Super::Deinitialize();

	if (URsPartyComponent* Model = CachedModel.Get())
	{
		Model->OnAddPartyMember.RemoveAll(this);
		Model->OnRemovePartyMember.RemoveAll(this);
	}
}

void URsPartyViewModel::HandleAddPartyMember(ARsPlayerCharacter* PartyMember, int32 MemberIndex)
{
	URsPlayerCharacterViewModel* NewCharacterViewModel = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(PartyMember);
	if (MemberIndex == 0)
	{
		SetPartyMemberViewModel_0(NewCharacterViewModel);
	}
	else if (MemberIndex == 1)
	{
		SetPartyMemberViewModel_1(NewCharacterViewModel);
	}
	else if (MemberIndex == 2)
	{
		SetPartyMemberViewModel_2(NewCharacterViewModel);
	}
}

void URsPartyViewModel::HandleRemovePartyMember(ARsPlayerCharacter* PartyMember, int32 MemberIndex)
{
	if (MemberIndex == 0)
	{
		SetPartyMemberViewModel_0(nullptr);
	}
	else if (MemberIndex == 1)
	{
		SetPartyMemberViewModel_1(nullptr);
	}
	else if (MemberIndex == 2)
	{
		SetPartyMemberViewModel_2(nullptr);
	}
}

void URsPartyViewModel::SetPartyMemberViewModel_0(URsPlayerCharacterViewModel* CharacterViewModel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_0, CharacterViewModel);
}

void URsPartyViewModel::SetPartyMemberViewModel_1(URsPlayerCharacterViewModel* CharacterViewModel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_1, CharacterViewModel);
}

void URsPartyViewModel::SetPartyMemberViewModel_2(URsPlayerCharacterViewModel* CharacterViewModel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_2, CharacterViewModel);
}