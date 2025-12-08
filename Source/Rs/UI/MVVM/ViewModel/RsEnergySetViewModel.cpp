// Copyright 2024 Team BH.


#include "RsEnergySetViewModel.h"

#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"

URsEnergySetViewModel* URsEnergySetViewModel::CreateEnergySetViewModel(const URsEnergySet* EnergySet)
{
	return CreateViewModel<URsEnergySetViewModel>(EnergySet);
}

void URsEnergySetViewModel::Initialize()
{
	Super::Initialize();
	
	// Data bindings
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxUltimateAttribute()).AddUObject(this, &ThisClass::MaxUltimateChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentUltimateAttribute()).AddUObject(this, &ThisClass::CurrentUltimateChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxManaAttribute()).AddUObject(this, &ThisClass::MaxManaChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentManaAttribute()).AddUObject(this, &ThisClass::CurrentManaChanged);
	}

	// Initial value set
	if (URsEnergySet* EnergySet = GetModel<ThisClass>())
	{
		SetMaxUltimate(EnergySet->GetMaxUltimate());
		SetCurrentUltimate(EnergySet->GetCurrentUltimate());
		SetMaxMana(EnergySet->GetMaxMana());
		SetCurrentMana(EnergySet->GetCurrentMana());
	}
}

void URsEnergySetViewModel::Deinitialize()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxUltimateAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentUltimateAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxManaAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentManaAttribute()).RemoveAll(this);
	}
	
	Super::Deinitialize();
}

float URsEnergySetViewModel::GetCurrentUltimate() const
{
	return CurrentUltimate;
}

void URsEnergySetViewModel::SetCurrentUltimate(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentUltimate, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetUltimatePercent);
	}
}

float URsEnergySetViewModel::GetMaxUltimate() const
{
	return MaxUltimate;
}

void URsEnergySetViewModel::SetMaxUltimate(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxUltimate, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetUltimatePercent);
	}
}

float URsEnergySetViewModel::GetUltimatePercent() const
{
	if (MaxUltimate != 0.f)
	{
		return CurrentUltimate / MaxUltimate;
	}
	else
	{
		return 0.f;
	}
}

float URsEnergySetViewModel::GetCurrentMana() const
{
	return CurrentMana;
}

void URsEnergySetViewModel::SetCurrentMana(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentMana, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetManaPercent);
	}
}

float URsEnergySetViewModel::GetMaxMana() const
{
	return MaxMana;
}

void URsEnergySetViewModel::SetMaxMana(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxMana, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetManaPercent);
	}
}

float URsEnergySetViewModel::GetManaPercent() const
{
	if (MaxMana != 0.f)
	{
		return CurrentMana / MaxMana;
	}
	else
	{
		return 0.f;
	}
}

void URsEnergySetViewModel::MaxUltimateChanged(const FOnAttributeChangeData& Data)
{
	SetMaxUltimate(Data.NewValue);
}

void URsEnergySetViewModel::CurrentUltimateChanged(const FOnAttributeChangeData& Data)
{
	SetCurrentUltimate(Data.NewValue);
}

void URsEnergySetViewModel::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	SetMaxMana(Data.NewValue);
}

void URsEnergySetViewModel::CurrentManaChanged(const FOnAttributeChangeData& Data)
{
	SetCurrentMana(Data.NewValue);
}
