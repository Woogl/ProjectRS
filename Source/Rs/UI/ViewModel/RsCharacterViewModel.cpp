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
	
	FString DisplayName = UKismetSystemLibrary::GetDisplayName(GetOuter());
	SetCharacterName(FText::FromString(DisplayName));
	
	ARsCharacterBase* Model = Cast<ARsCharacterBase>(GetOuter());
	HealthSetViewModel = URsHealthSetViewModel::CreateHealthSetViewModel(Model);
	StaggerSetViewModel = URsStaggerSetViewModel::CreateStaggerSetViewModel(Model);
}

void URsCharacterViewModel::Deinitialize()
{
	Super::Deinitialize();
}

FText URsCharacterViewModel::GetCharacterName() const
{
	return CharacterName;
}

void URsCharacterViewModel::SetCharacterName(FText NewCharacterName)
{
	UE_MVVM_SET_PROPERTY_VALUE(CharacterName, NewCharacterName);
}
