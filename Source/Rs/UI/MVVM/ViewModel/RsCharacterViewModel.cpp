// Copyright 2024 Team BH.


#include "RsCharacterViewModel.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "CommonHardwareVisibilityBorder.h"
#include "RsActiveEffectListViewViewModel.h"
#include "RsHealthSetViewModel.h"
#include "RsStaggerSetViewModel.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"
#include "Rs/Character/RsCharacterBase.h"

URsCharacterViewModel* URsCharacterViewModel::CreateRsCharacterViewModel(ARsCharacterBase* Character)
{
	return CreateViewModel<URsCharacterViewModel>(Character);
}

bool URsCharacterViewModel::TryActivateAbility(FGameplayTag AbilityTag)
{
	if (const ARsCharacterBase* Character = GetModel<ThisClass>())
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

	if (const ARsCharacterBase* Character = GetModel<ThisClass>())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character))
		{
			if (const URsHealthSet* HealthSet = ASC->GetSet<URsHealthSet>())
			{
				UE_MVVM_SET_PROPERTY_VALUE(HealthSetViewModel, URsHealthSetViewModel::CreateHealthSetViewModel(HealthSet));
			}
			if (const URsStaggerSet* StaggerSet = ASC->GetSet<URsStaggerSet>())
			{
				UE_MVVM_SET_PROPERTY_VALUE(StaggerSetViewModel, URsStaggerSetViewModel::CreateStaggerSetViewModel(StaggerSet));
			}
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
	if (const ARsCharacterBase* Character = GetModel<ThisClass>())
	{
		return FText::FromString(UKismetSystemLibrary::GetDisplayName(Character));
	}
	return FText::GetEmpty();
}

UObject* URsCharacterViewModel::GetPortrait() const
{
	if (const ARsCharacterBase* Character = GetModel<ThisClass>())
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
