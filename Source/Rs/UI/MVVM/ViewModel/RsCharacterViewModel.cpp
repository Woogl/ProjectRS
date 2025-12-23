// Copyright 2024 Team BH.


#include "RsCharacterViewModel.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "RsActiveEffectViewModel.h"
#include "RsEnergySetViewModel.h"
#include "RsHealthSetViewModel.h"
#include "RsStaggerSetViewModel.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"
#include "Rs/Character/RsCharacterBase.h"

URsCharacterViewModel* URsCharacterViewModel::CreateRsCharacterViewModel(ARsCharacterBase* Character)
{
	return CreateViewModel<URsCharacterViewModel>(Character);
}

bool URsCharacterViewModel::TryActivateAbility(FGameplayTag AbilityTag)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		return ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
	}
	return false;
}

void URsCharacterViewModel::Initialize()
{
	Super::Initialize();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (const URsHealthSet* HealthSet = ASC->GetSet<URsHealthSet>())
		{
			UE_MVVM_SET_PROPERTY_VALUE(HealthSetViewModel, URsHealthSetViewModel::CreateHealthSetViewModel(HealthSet));
		}
		if (const URsStaggerSet* StaggerSet = ASC->GetSet<URsStaggerSet>())
		{
			UE_MVVM_SET_PROPERTY_VALUE(StaggerSetViewModel, URsStaggerSetViewModel::CreateStaggerSetViewModel(StaggerSet));
		}
		if (const URsEnergySet* EnergySet = ASC->GetSet<URsEnergySet>())
		{
			UE_MVVM_SET_PROPERTY_VALUE(EnergySetViewModel, URsEnergySetViewModel::CreateEnergySetViewModel(EnergySet));
		}
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::HandleEffectAdded);
	}
}

void URsCharacterViewModel::Deinitialize()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		ASC->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
	}
	Super::Deinitialize();
}

void URsCharacterViewModel::HandleEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	if (URsActiveEffectViewModel* NewEffectViewModel = URsActiveEffectViewModel::CreateRsActiveEffectViewModel(EffectHandle))
	{
		ActiveEffectViewModels.Add(NewEffectViewModel);
		ActiveEffectViewModels.Sort([](const URsActiveEffectViewModel& A, const URsActiveEffectViewModel& B)
		{
			return A.GetPriority() > B.GetPriority();
		});
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ActiveEffectViewModels);
		
		ASC->OnGameplayEffectRemoved_InfoDelegate(EffectHandle)->AddUObject(this, &ThisClass::HandleEffectRemoved);
	}
}

void URsCharacterViewModel::HandleEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		const FActiveGameplayEffect* RemovalEffect = ASC->GetActiveGameplayEffect(RemovalInfo.ActiveEffect->Handle);
		if (RemovalEffect)
		{
			UE_LOG(LogRsAbility, Error, TEXT("Not removed effect : %s"), *RemovalInfo.ActiveEffect->Spec.ToSimpleString());
			return;
		}

		for (TObjectPtr<URsActiveEffectViewModel> ActiveEffectViewModel : ActiveEffectViewModels)
		{
			if (ActiveEffectViewModel->GetActiveEffect() == RemovalEffect)
			{
				ActiveEffectViewModels.Remove(*ActiveEffectViewModel);
				UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ActiveEffectViewModels);
				break;
			}
		}
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
		if (Character->CharacterName.IsEmpty())
		{
			UE_LOG(LogRs, Warning, TEXT("Character %s has no name."), *Character->GetName());
			return FText::FromString(UKismetSystemLibrary::GetDisplayName(Character));
		}
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

UAbilitySystemComponent* URsCharacterViewModel::GetAbilitySystemComponent() const
{
	if (const ARsCharacterBase* Character = GetModel<ThisClass>())
	{
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character);
	}
	return nullptr;		
}
