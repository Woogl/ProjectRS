// Copyright 2024 Team BH.


#include "RsPartyViewModel.h"

#include "RsPlayerCharacterViewModel.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/UI/MVVM/RsMVVMGameSubsystem.h"

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
			PartyMemberViewModels.Add(CharacterViewModel);
		}
	}

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		PlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::HandlePossessedPawnChanged);
	}
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(PartyMemberViewModels);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentPartyMember);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentPartyMemberIndex);
}

void URsPartyViewModel::Deinitialize()
{
	if (URsPartyComponent* PartyComponent = GetModel<ThisClass>())
	{
		PartyComponent->OnAddPartyMember.RemoveAll(this);
		PartyComponent->OnRemovePartyMember.RemoveAll(this);
	}

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		PlayerController->OnPossessedPawnChanged.RemoveAll(this);
	}
	Super::Deinitialize();
}

URsPlayerCharacterViewModel* URsPartyViewModel::GetCurrentPartyMember() const
{
	return PartyMemberViewModels.IsEmpty() ? nullptr : PartyMemberViewModels[GetCurrentPartyMemberIndex()];
}

URsPlayerCharacterViewModel* URsPartyViewModel::GetPrevPartyMember() const
{
	return PartyMemberViewModels.IsEmpty() ? nullptr : PartyMemberViewModels[(GetCurrentPartyMemberIndex() - 1 + PartyMemberViewModels.Num()) % PartyMemberViewModels.Num()];
}

URsPlayerCharacterViewModel* URsPartyViewModel::GetNextPartyMember() const
{
	return PartyMemberViewModels.IsEmpty() ? nullptr :PartyMemberViewModels[(GetCurrentPartyMemberIndex() + 1) % PartyMemberViewModels.Num()];
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

void URsPartyViewModel::SwitchPartyMember(int32 MemberIndex)
{
	if (URsPartyComponent* PartyComponent = GetModel<ThisClass>())
	{
		PartyComponent->SwitchPartyMember(MemberIndex);
	}
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
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(PartyMemberViewModels);
		}
	}
}

void URsPartyViewModel::HandleAddPartyMember(ARsPlayerCharacter* PartyMember, int32 MemberIndex)
{
	URsPlayerCharacterViewModel* NewCharacterViewModel = URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(PartyMember);
	PartyMemberViewModels.Add(NewCharacterViewModel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(PartyMemberViewModels);
}

void URsPartyViewModel::HandleRemovePartyMember(ARsPlayerCharacter* PartyMember, int32 MemberIndex)
{
	if (MemberIndex < PartyMemberViewModels.Num())
	{
		PartyMemberViewModels.RemoveAt(MemberIndex);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(PartyMemberViewModels);
	}
}

UObject* URsPartyViewModelResolver::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const
{
	ULocalPlayer* LocalPlayer = UserWidget->GetOwningLocalPlayer();
	if (!LocalPlayer)
	{
		return nullptr;
	}

	URsPartyComponent* PartyComponent = URsPartyLibrary::GetPartyComponent(LocalPlayer);
	if (!PartyComponent)
	{
		return nullptr;
	}

	return URsSingletonViewModelBase::GetOrCreateViewModel<URsPartyViewModel>(PartyComponent);
}
