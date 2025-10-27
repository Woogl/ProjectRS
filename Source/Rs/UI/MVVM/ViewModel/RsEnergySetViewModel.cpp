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
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxUltimateAttribute()).AddUObject(this, &ThisClass::MaxUltimateChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentUltimateAttribute()).AddUObject(this, &ThisClass::CurrentUltimateChanged);
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

float URsEnergySetViewModel::GetCurrentUltimate() const
{
	if (URsEnergySet* EnergySet = GetModel<ThisClass>())
	{
		return EnergySet->GetCurrentUltimate();
	}
	return 0.f;
}

float URsEnergySetViewModel::GetMaxUltimate() const
{
	if (URsEnergySet* EnergySet = GetModel<ThisClass>())
	{
		return EnergySet->GetMaxUltimate();
	}
	return 0.f;
}

float URsEnergySetViewModel::GetUltimatePercent() const
{
	if (GetMaxUltimate() != 0.f)
	{
		return GetCurrentUltimate() / GetMaxUltimate();
	}
	else
	{
		return 0.f;
	}
}

void URsEnergySetViewModel::MaxUltimateChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxUltimate);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetUltimatePercent);
}

void URsEnergySetViewModel::CurrentUltimateChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentUltimate);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetUltimatePercent);
}
