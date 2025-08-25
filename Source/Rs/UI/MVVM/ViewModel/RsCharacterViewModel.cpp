// Copyright 2024 Team BH.


#include "RsCharacterViewModel.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "CommonHardwareVisibilityBorder.h"
#include "RsActiveEffectListViewViewModel.h"
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

bool URsCharacterViewModel::TryActivateAbility(FGameplayTag AbilityTag)
{
	if (ARsCharacterBase* Model = CachedModel.Get())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Model))
		{
			return ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
		}
	}
	return false;
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
			UE_MVVM_SET_PROPERTY_VALUE(ActiveEffectListViewViewModel, URsActiveEffectListViewViewModel::CreateActiveEffectListViewViewModel(ASC));
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

UObject* URsCharacterViewModel::GetPortrait() const
{
	if (ARsCharacterBase* Model = CachedModel.Get())
	{
		return Model->Portrait;
	}
	return nullptr;
}

ESlateVisibility URsCharacterViewModel::GetPortraitVisibility() const
{
	if (GetPortrait())
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Hidden;
}
