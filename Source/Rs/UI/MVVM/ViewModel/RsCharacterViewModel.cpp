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
	return CreateViewModel<URsCharacterViewModel>(Character);
}

bool URsCharacterViewModel::TryActivateAbility(FGameplayTag AbilityTag)
{
	if (ARsCharacterBase* Character = GetModel<ARsCharacterBase>())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character))
		{
			return ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
		}
	}
	return false;
}

void URsCharacterViewModel::Initialize()
{
	Super::Initialize();

	if (ARsCharacterBase* Character = GetModel<ARsCharacterBase>())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character))
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
	if (ARsCharacterBase* Character = GetModel<ARsCharacterBase>())
	{
		return FText::FromString(UKismetSystemLibrary::GetDisplayName(Character));
	}
	return FText::GetEmpty();
}

UObject* URsCharacterViewModel::GetPortrait() const
{
	if (ARsCharacterBase* Character = GetModel<ARsCharacterBase>())
	{
		return Character->Portrait;
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
