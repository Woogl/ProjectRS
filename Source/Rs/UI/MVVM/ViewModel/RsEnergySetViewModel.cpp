// Copyright 2024 Team BH.


#include "RsEnergySetViewModel.h"

#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"

URsEnergySetViewModel* URsEnergySetViewModel::CreateEnergySetViewModel(UAbilitySystemComponent* ASC)
{
	return CreateViewModel<URsEnergySetViewModel>(ASC);
}

void URsEnergySetViewModel::Initialize()
{
	Super::Initialize();
	
	if (UAbilitySystemComponent* ASC = GetModel<UAbilitySystemComponent>())
	{
		SetMaxEnergy(ASC->GetNumericAttribute(URsEnergySet::GetMaxEnergyAttribute()));
		SetCurrentEnergy(ASC->GetNumericAttribute(URsEnergySet::GetCurrentEnergyAttribute()));
		
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxEnergyAttribute()).AddUObject(this, &ThisClass::MaxEnergyChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentEnergyAttribute()).AddUObject(this, &ThisClass::CurrentEnergyChanged);
	}
}

void URsEnergySetViewModel::Deinitialize()
{
	Super::Deinitialize();
	
	if (UAbilitySystemComponent* ASC = GetModel<UAbilitySystemComponent>())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxEnergyAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentEnergyAttribute()).RemoveAll(this);
	}
}

float URsEnergySetViewModel::GetCurrentEnergy() const
{
	return CurrentEnergy;
}

float URsEnergySetViewModel::GetMaxEnergy() const
{
	return CurrentEnergy;
}

float URsEnergySetViewModel::GetEnergyRegen() const
{
	return EnergyRegen;
}

void URsEnergySetViewModel::SetCurrentEnergy(float NewCurrentEnergy)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentEnergy, NewCurrentEnergy))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEnergyPercent);
	}
}

void URsEnergySetViewModel::SetMaxEnergy(float NewMaxEnergy)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxEnergy, NewMaxEnergy))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEnergyPercent);
	}
}

void URsEnergySetViewModel::SetEnergyRegen(float NewEnergyRegen)
{
	UE_MVVM_SET_PROPERTY_VALUE(EnergyRegen, NewEnergyRegen);
}

float URsEnergySetViewModel::GetEnergyPercent() const
{
	if (MaxEnergy != 0)
	{
		return CurrentEnergy / MaxEnergy;
	}
	else
	{
		return 0;
	}
}

void URsEnergySetViewModel::MaxEnergyChanged(const FOnAttributeChangeData& Data)
{
	SetMaxEnergy(Data.NewValue);
}

void URsEnergySetViewModel::CurrentEnergyChanged(const FOnAttributeChangeData& Data)
{
	SetCurrentEnergy(Data.NewValue);
}

void URsEnergySetViewModel::EnergyRegenChanged(const FOnAttributeChangeData& Data)
{
	SetEnergyRegen(Data.NewValue);
}
