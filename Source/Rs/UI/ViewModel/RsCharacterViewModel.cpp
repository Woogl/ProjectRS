// Copyright 2024 Team BH.


#include "RsCharacterViewModel.h"

#include "AbilitySystemGlobals.h"
#include "RsHealthSetViewModel.h"
#include "RsStaggerSetViewModel.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsCharacterBase.h"

URsCharacterViewModel* URsCharacterViewModel::CreateRsCharacterViewModel(ARsCharacterBase* Character)
{
	URsCharacterViewModel* ViewModel = NewObject<URsCharacterViewModel>(Character);
	ViewModel->Initialize();
	return ViewModel;
}

void URsCharacterViewModel::Initialize()
{
	Super::Initialize();

	CachedModel = Cast<ARsCharacterBase>(GetOuter());
	if (ARsCharacterBase* Model = CachedModel.Get())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Model))
		{
			UE_MVVM_SET_PROPERTY_VALUE(HealthSetViewModel, URsHealthSetViewModel::CreateHealthSetViewModel(ASC));
			UE_MVVM_SET_PROPERTY_VALUE(StaggerSetViewModel, URsStaggerSetViewModel::CreateStaggerSetViewModel(ASC));
		}
	}
}

void URsCharacterViewModel::Deinitialize()
{
	Super::Deinitialize();
}

FText URsCharacterViewModel::GetCharacterName() const
{
	if (ARsCharacterBase* Model = CachedModel.Get())
	{
		return FText::FromString(UKismetSystemLibrary::GetDisplayName(Model));
	}
	return FText::GetEmpty();
}

UObject* URsCharacterViewModel::GetCharacterIcon() const
{
	if (ARsCharacterBase* Model = CachedModel.Get())
	{
		return Model->CharacterIcon;
	}
	return nullptr;
}

bool URsCharacterViewModel::HasCharacterIcon() const
{
	return GetCharacterIcon() != nullptr;
}
