// Copyright 2024 Team BH.


#include "RsPartyViewModel.h"

#include "CommonHardwareVisibilityBorder.h"
#include "RsPlayerCharacterViewModel.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/UI/MVVM/RsMVVMGameSubsystem.h"

URsPartyViewModel* URsPartyViewModel::GetPartyViewModel(UObject* WorldContext)
{
	return GetSingletonViewModel<URsPartyViewModel>(WorldContext);
}

bool URsPartyViewModel::TrySwitchMemberAbility(int32 MemberIndex)
{
	if (URsPartyComponent* PartyComponent = GetModel<ThisClass>())
	{
		return PartyComponent->TrySwitchMemberAbility(MemberIndex);
	}
	return false;
}

void URsPartyViewModel::Initialize()
{
	Super::Initialize();

	if (URsPartyComponent* PartyComponent = GetModel<ThisClass>())
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

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		PlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::HandlePossessedPawnChanged);
	}
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentPartyMember);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentPartyMemberIndex);
}

void URsPartyViewModel::Deinitialize()
{
	Super::Deinitialize();

	if (URsPartyComponent* PartyComponent = GetModel<ThisClass>())
	{
		PartyComponent->OnAddPartyMember.RemoveAll(this);
		PartyComponent->OnRemovePartyMember.RemoveAll(this);
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

URsPlayerCharacterViewModel* URsPartyViewModel::GetCurrentPartyMember() const
{
	int32 CurrentIndex = GetCurrentPartyMemberIndex();
	if (CurrentIndex == 0)
	{
		return PartyMemberViewModel_0;
	}
	else if (CurrentIndex == 1)
	{
		return PartyMemberViewModel_1;
	}
	else if (CurrentIndex == 2)
	{
		return PartyMemberViewModel_2;
	}
	else
	{
		if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			return URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(Cast<ARsPlayerCharacter>(Character));
		}
	}
	return nullptr;
}

URsPlayerCharacterViewModel* URsPartyViewModel::GetPrevPartyMember() const
{
	int32 CurrentIndex = GetCurrentPartyMemberIndex();
	if (CurrentIndex == 0)
	{
		return PartyMemberViewModel_2;
	}
	else if (CurrentIndex == 1)
	{
		return PartyMemberViewModel_0;
	}
	else if (CurrentIndex == 2)
	{
		return PartyMemberViewModel_1;
	}
	return nullptr;
}

URsPlayerCharacterViewModel* URsPartyViewModel::GetNextPartyMember() const
{
	int32 CurrentIndex = GetCurrentPartyMemberIndex();
	if (CurrentIndex == 0)
	{
		return PartyMemberViewModel_1;
	}
	else if (CurrentIndex == 1)
	{
		return PartyMemberViewModel_2;
	}
	else if (CurrentIndex == 2)
	{
		return PartyMemberViewModel_0;
	}
	return nullptr;
}

URsPlayerCharacterViewModel* URsPartyViewModel::GetPrevAlivePartyMember() const
{
	if (URsPartyComponent* PartyComponent = GetModel<ThisClass>())
	{
		if (ARsPlayerCharacter* Character = PartyComponent->GetPrevAlivePartyMember())
		{
			return URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(Character);
		}
	}
	return nullptr;
}

URsPlayerCharacterViewModel* URsPartyViewModel::GetNextAlivePartyMember() const
{
	if (URsPartyComponent* PartyComponent = GetModel<ThisClass>())
	{
		if (ARsPlayerCharacter* Character = PartyComponent->GetNextAlivePartyMember())
		{
			return URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(Character);
		}
	}
	return nullptr;
}

int32 URsPartyViewModel::GetCurrentPartyMemberIndex() const
{
	if (URsPartyComponent* PartyComponent = GetModel<ThisClass>())
	{
		return PartyComponent->GetCurrentMemberIndex();
	}
	return INDEX_NONE;
}

void URsPartyViewModel::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (URsPartyComponent* PartyComponent = GetModel<ThisClass>())
	{
		int32 Index = PartyComponent->GetPartyMembers().Find(Cast<ARsPlayerCharacter>(NewPawn));
		if (Index != INDEX_NONE)
		{
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentPartyMember);
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentPartyMemberIndex);
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPrevPartyMember);
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetNextPartyMember);
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