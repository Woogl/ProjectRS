// Copyright 2024 Team BH.


#include "RsEnergySetViewModel.h"

#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"

URsEnergySetViewModel* URsEnergySetViewModel::CreateEnergySetViewModel(URsEnergySet* EnergySet)
{
	return CreateViewModel<URsEnergySetViewModel>(EnergySet);
}

void URsEnergySetViewModel::Initialize()
{
	Super::Initialize();
	
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxEnergyAttribute()).AddUObject(this, &ThisClass::MaxEnergyChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentEnergyAttribute()).AddUObject(this, &ThisClass::CurrentEnergyChanged);
	}
}

void URsEnergySetViewModel::Deinitialize()
{
	Super::Deinitialize();
	
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxEnergyAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentEnergyAttribute()).RemoveAll(this);
	}
}

float URsEnergySetViewModel::GetCurrentEnergy() const
{
	if (URsEnergySet* EnergySet = GetModel<URsEnergySet>())
	{
		return EnergySet->GetCurrentEnergy();
	}
	return 0.f;
}

float URsEnergySetViewModel::GetMaxEnergy() const
{
	if (URsEnergySet* EnergySet = GetModel<URsEnergySet>())
	{
		return EnergySet->GetMaxEnergy();
	}
	return 0.f;
}

float URsEnergySetViewModel::GetEnergyPercent() const
{
	if (GetMaxEnergy() != 0)
	{
		return GetCurrentEnergy() / GetMaxEnergy();
	}
	else
	{
		return 0;
	}
}

void URsEnergySetViewModel::MaxEnergyChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxEnergy);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEnergyPercent);
}

void URsEnergySetViewModel::CurrentEnergyChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentEnergy);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEnergyPercent);
}
