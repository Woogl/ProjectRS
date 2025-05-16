// Copyright 2024 Team BH.


#include "RsPartyViewModel.h"

#include "RsPlayerCharacterViewModel.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/Party/RsPartyLibrary.h"

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

	if (URsPartyComponent* PartyComponent = Cast<URsPartyComponent>(GetOuter()))
	{
		PartyComponent->OnAddPartyMember.RemoveAll(this);
		PartyComponent->OnRemovePartyMember.RemoveAll(this);
	}
}

bool URsPartyViewModel::HasPartyMemberInSlot0() const
{
	return URsPartyLibrary::GetPartyMemberAt(GetWorld(), 0) != nullptr;
}

bool URsPartyViewModel::HasPartyMemberInSlot1() const
{
	return URsPartyLibrary::GetPartyMemberAt(GetWorld(), 1) != nullptr;
}

bool URsPartyViewModel::HasPartyMemberInSlot2() const
{
	return URsPartyLibrary::GetPartyMemberAt(GetWorld(), 2) != nullptr;
}

void URsPartyViewModel::SetPartyMemberViewModel_0(URsPlayerCharacterViewModel* CharacterViewModel)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_0, CharacterViewModel))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HasPartyMemberInSlot0);
	}
}

void URsPartyViewModel::SetPartyMemberViewModel_1(URsPlayerCharacterViewModel* CharacterViewModel)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_1, CharacterViewModel))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HasPartyMemberInSlot1);
	}
}

void URsPartyViewModel::SetPartyMemberViewModel_2(URsPlayerCharacterViewModel* CharacterViewModel)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_2, CharacterViewModel))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HasPartyMemberInSlot2);
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
