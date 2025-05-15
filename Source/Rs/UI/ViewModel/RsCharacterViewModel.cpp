// Copyright 2024 Team BH.


#include "RsCharacterViewModel.h"

#include "RsHealthSetViewModel.h"
#include "RsStaggerSetViewModel.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsCharacterBase.h"

URsCharacterViewModel* URsCharacterViewModel::CreateRsCharacterViewModel(ARsCharacterBase* Model)
{
	URsCharacterViewModel* ViewModel = NewObject<URsCharacterViewModel>(Model);
	ViewModel->Initialize();
	return ViewModel;
}

void URsCharacterViewModel::Initialize()
{
	Super::Initialize();
	
	if (ARsCharacterBase* Model = Cast<ARsCharacterBase>(GetOuter()))
	{
		FString DisplayName = UKismetSystemLibrary::GetDisplayName(GetOuter());
		SetCharacterName(FText::FromString(DisplayName));
		SetCharacterIcon(Model->CharacterIcon);
		
		HealthSetViewModel = URsHealthSetViewModel::CreateHealthSetViewModel(Model);
		StaggerSetViewModel = URsStaggerSetViewModel::CreateStaggerSetViewModel(Model);
	}
}

void URsCharacterViewModel::Deinitialize()
{
	Super::Deinitialize();
}

FText URsCharacterViewModel::GetCharacterName() const
{
	return CharacterName;
}

UObject* URsCharacterViewModel::GetCharacterIcon() const
{
	return CharacterIcon;
}

void URsCharacterViewModel::SetCharacterName(FText NewCharacterName)
{
	UE_MVVM_SET_PROPERTY_VALUE(CharacterName, NewCharacterName);
}

void URsCharacterViewModel::SetCharacterIcon(UObject* NewCharacterIcon)
{
	UE_MVVM_SET_PROPERTY_VALUE(CharacterIcon, NewCharacterIcon);
}
