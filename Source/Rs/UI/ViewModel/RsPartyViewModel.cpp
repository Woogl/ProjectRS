// Copyright 2024 Team BH.


#include "RsPartyViewModel.h"

#include "CommonHardwareVisibilityBorder.h"
#include "RsPlayerCharacterViewModel.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyComponent.h"

URsPartyViewModel* URsPartyViewModel::CreateRsPartyViewModel(URsPartyComponent* PartyComponent)
{
	URsPartyViewModel* ViewModel = NewObject<URsPartyViewModel>(PartyComponent);
	ViewModel->Initialize();
	return ViewModel;
}

bool URsPartyViewModel::TrySwitchMemberAbility(int32 MemberIndex)
{
	if (URsPartyComponent* Model = CachedModel.Get())
	{
		return Model->TrySwitchMemberAbility(MemberIndex);
	}
	return false;
}

void URsPartyViewModel::Initialize()
{
	Super::Initialize();

	CachedModel = Cast<URsPartyComponent>(GetOuter());
	if (URsPartyComponent* Model = CachedModel.Get())
	{
		if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			if (ARsPlayerCharacter* RsCharacter = Cast<ARsPlayerCharacter>(PlayerCharacter))
			{
				ControlledPartyMemberIndex = Model->GetPartyMembers().Find(RsCharacter);
			}
		}
		
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

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		PlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::HandlePossessedPawnChanged);
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

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		PlayerController->OnPossessedPawnChanged.RemoveAll(this);
	}
}

ESlateVisibility URsPartyViewModel::GetPartyMemberVisibility_0() const
{
	if (PartyMemberViewModel_0)
	{
		return ESlateVisibility::Visible;
	}
	return ESlateVisibility::Hidden;
}

ESlateVisibility URsPartyViewModel::GetPartyMemberVisibility_1() const
{
	if (PartyMemberViewModel_1)
	{
		return ESlateVisibility::Visible;
	}
	return ESlateVisibility::Hidden;
}

ESlateVisibility URsPartyViewModel::GetPartyMemberVisibility_2() const
{
	if (PartyMemberViewModel_2)
	{
		return ESlateVisibility::Visible;
	}
	return ESlateVisibility::Hidden;
}

URsPlayerCharacterViewModel* URsPartyViewModel::GetPreviousPartyMemberViewModel() const
{
	if (ControlledPartyMemberIndex == 0)
	{
		return PartyMemberViewModel_2;
	}
	else if (ControlledPartyMemberIndex == 1)
	{
		return PartyMemberViewModel_0;
	}
	else if (ControlledPartyMemberIndex == 2)
	{
		return PartyMemberViewModel_1;
	}
	return nullptr;
}

URsPlayerCharacterViewModel* URsPartyViewModel::GetControlledPartyMemberViewModel() const
{
	if (ControlledPartyMemberIndex == 0)
	{
		return PartyMemberViewModel_0;
	}
	else if (ControlledPartyMemberIndex == 1)
	{
		return PartyMemberViewModel_1;
	}
	else if (ControlledPartyMemberIndex == 2)
	{
		return PartyMemberViewModel_2;
	}
	return nullptr;
}

URsPlayerCharacterViewModel* URsPartyViewModel::GetNextPartyMemberViewModel() const
{
	if (ControlledPartyMemberIndex == 0)
	{
		return PartyMemberViewModel_1;
	}
	else if (ControlledPartyMemberIndex == 1)
	{
		return PartyMemberViewModel_2;
	}
	else if (ControlledPartyMemberIndex == 2)
	{
		return PartyMemberViewModel_0;
	}
	return nullptr;
}

void URsPartyViewModel::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (URsPartyComponent* Model = CachedModel.Get())
	{
		int32 Index = Model->GetPartyMembers().Find(Cast<ARsPlayerCharacter>(NewPawn));
		if (Index != INDEX_NONE)
		{
			SetControlledPartyMemberIndex(Index);
		}
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

void URsPartyViewModel::SetControlledPartyMemberIndex(int32 Index)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(ControlledPartyMemberIndex, Index))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPreviousPartyMemberViewModel);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetControlledPartyMemberViewModel);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetNextPartyMemberViewModel);
	}
}

void URsPartyViewModel::SetPartyMemberViewModel_0(URsPlayerCharacterViewModel* CharacterViewModel)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_0, CharacterViewModel))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartyMemberVisibility_0);
	}
}

void URsPartyViewModel::SetPartyMemberViewModel_1(URsPlayerCharacterViewModel* CharacterViewModel)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_1, CharacterViewModel))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartyMemberVisibility_1);
	}
}

void URsPartyViewModel::SetPartyMemberViewModel_2(URsPlayerCharacterViewModel* CharacterViewModel)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(PartyMemberViewModel_2, CharacterViewModel))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartyMemberVisibility_2);
	}
}