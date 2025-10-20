// Copyright 2024 Team BH.


#include "RsCharacterViewModel.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "RsActiveEffectViewModel.h"
#include "RsHealthSetViewModel.h"
#include "RsStaggerSetViewModel.h"
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
			ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::OnEffectAdded);
		}
	}
}

void URsCharacterViewModel::Deinitialize()
{
	if (const ARsCharacterBase* Character = GetModel<ThisClass>())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character))
		{
			ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		}
	}
	Super::Deinitialize();
}

void URsCharacterViewModel::OnEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	if (URsActiveEffectViewModel* NewEffectViewModel = URsActiveEffectViewModel::CreateRsActiveEffectViewModel(EffectHandle))
	{
		NewEffectViewModel->OnViewModelDisabled.BindUObject(this, &ThisClass::OnEffectRemoved);
		ActiveEffectViewModels.Add(NewEffectViewModel);
		ActiveEffectViewModels.Sort([](const URsActiveEffectViewModel& A, const URsActiveEffectViewModel& B)->bool{return A.GetPriority() > B.GetPriority();});
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ActiveEffectViewModels);
	}
}

void URsCharacterViewModel::OnEffectRemoved(URsActiveEffectViewModel* DisabledViewModel)
{
	if (DisabledViewModel)
	{
		DisabledViewModel->OnViewModelDisabled.Unbind();
		ActiveEffectViewModels.Remove(DisabledViewModel);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ActiveEffectViewModels);
	}
}

UObject* URsCharacterViewModel::GetPortrait() const
{
	if (const ARsCharacterBase* Character = GetModel<ThisClass>())
	{
		return Character->Portrait;
	}
	return nullptr;
}

FText URsCharacterViewModel::GetCharacterName() const
{
	if (const ARsCharacterBase* Character = GetModel<ThisClass>())
	{
		return Character->CharacterName;
	}
	return FText::GetEmpty();
}

FText URsCharacterViewModel::GetDescription() const
{
	if (const ARsCharacterBase* Character = GetModel<ThisClass>())
	{
		return Character->Description;
	}
	return FText::GetEmpty();
}

ESlateVisibility URsCharacterViewModel::GetPortraitVisibility() const
{
	if (GetPortrait())
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Hidden;
}
