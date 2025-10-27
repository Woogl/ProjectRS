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
	
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxUltimateAttribute()).AddUObject(this, &ThisClass::MaxEnergyChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentUltimateAttribute()).AddUObject(this, &ThisClass::CurrentEnergyChanged);
	}
}

void URsEnergySetViewModel::Deinitialize()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxUltimateAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentUltimateAttribute()).RemoveAll(this);
	}
	Super::Deinitialize();
}

float URsEnergySetViewModel::GetCurrentEnergy() const
{
	if (URsEnergySet* EnergySet = GetModel<ThisClass>())
	{
		return EnergySet->GetCurrentUltimate();
	}
	return 0.f;
}

float URsEnergySetViewModel::GetMaxEnergy() const
{
	if (URsEnergySet* EnergySet = GetModel<ThisClass>())
	{
		return EnergySet->GetMaxUltimate();
	}
	return 0.f;
}

float URsEnergySetViewModel::GetEnergyPercent() const
{
	if (GetMaxEnergy() != 0.f)
	{
		return GetCurrentEnergy() / GetMaxEnergy();
	}
	else
	{
		return 0.f;
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
