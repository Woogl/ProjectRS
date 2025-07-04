// Copyright 2025 Team BH.


#include "RsActiveEffectListViewViewModel.h"
#include "AbilitySystemComponent.h"
#include "CommonHardwareVisibilityBorder.h"
#include "RsActiveEffectViewModel.h"

URsActiveEffectListViewViewModel* URsActiveEffectListViewViewModel::CreateActiveEffectListViewViewModel(UAbilitySystemComponent* ASC)
{
	URsActiveEffectListViewViewModel* ViewModel = NewObject<URsActiveEffectListViewViewModel>(ASC);
	ViewModel->Initialize();
	return ViewModel;
}

void URsActiveEffectListViewViewModel::Initialize()
{
	Super::Initialize();

	CachedModel = Cast<UAbilitySystemComponent>(GetOuter());
	if (CachedModel.IsValid())
		{
		UE_MVVM_SET_PROPERTY_VALUE(Visibility, ESlateVisibility::SelfHitTestInvisible);
		CachedModel.Get()->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::OnEffectAdded);
	}
}

void URsActiveEffectListViewViewModel::Deinitialize()
{
	Super::Deinitialize();
}

void URsActiveEffectListViewViewModel::OnEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	if (URsActiveEffectViewModel* NewEffectViewModel = URsActiveEffectViewModel::CreateRsActiveEffectViewModel(EffectHandle))
	{
		FGameplayTagContainer EffectTags;
		EffectSpec.GetAllAssetTags(EffectTags);
		// if same effect exists already
		if (CachedModel.Get()->GetActiveEffectsWithAllTags(EffectTags).Num() > 1)
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
