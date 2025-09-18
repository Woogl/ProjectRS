// Copyright 2025 Team BH.


#include "RsActiveEffectListViewViewModel.h"
#include "AbilitySystemComponent.h"
#include "RsActiveEffectViewModel.h"

URsActiveEffectListViewViewModel* URsActiveEffectListViewViewModel::CreateActiveEffectListViewViewModel(UAbilitySystemComponent* ASC)
{
	return CreateViewModel<URsActiveEffectListViewViewModel>(ASC);
}

void URsActiveEffectListViewViewModel::Initialize()
{
	Super::Initialize();
	
	if (UAbilitySystemComponent* ASC = GetModel<ThisClass>())
	{
		UE_MVVM_SET_PROPERTY_VALUE(Visibility, ESlateVisibility::SelfHitTestInvisible);
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::OnEffectAdded);
	}
}

void URsActiveEffectListViewViewModel::Deinitialize()
{
	Super::Deinitialize();
	
	if (UAbilitySystemComponent* ASC = GetModel<ThisClass>())
	{
		UE_MVVM_SET_PROPERTY_VALUE(Visibility, ESlateVisibility::SelfHitTestInvisible);
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
	}
}

void URsActiveEffectListViewViewModel::OnEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	if (URsActiveEffectViewModel* NewEffectViewModel = URsActiveEffectViewModel::CreateRsActiveEffectViewModel(EffectHandle))
	{
		FGameplayTagContainer EffectTags;
		EffectSpec.GetAllAssetTags(EffectTags);
		// if same effect exists already
		if (GetModel<ThisClass>()->GetActiveEffectsWithAllTags(EffectTags).Num() > 1)
		{
			// stack update for every existing viewmodels
			for (URsActiveEffectViewModel* ActiveEffectViewModel : ActiveEffectViewModels)
			{
				ActiveEffectViewModel->AddExtraModel(EffectHandle);
			}
		}
		else
		{
			// add new viewmodel
			NewEffectViewModel->OnViewModelDisabled.AddUObject(this, &ThisClass::OnEffectRemoved);
			ActiveEffectViewModels.Add(NewEffectViewModel);
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ActiveEffectViewModels);
		}
	}
}

void URsActiveEffectListViewViewModel::OnEffectRemoved(URsActiveEffectViewModel* DisabledViewModel)
{
	if (DisabledViewModel)
	{
		DisabledViewModel->OnViewModelDisabled.RemoveAll(this);
		ActiveEffectViewModels.Remove(DisabledViewModel);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ActiveEffectViewModels);
	}
}
