// Copyright 2024 Team BH.


#include "RsEnergySetViewModel.h"

#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"

URsEnergySetViewModel* URsEnergySetViewModel::CreateEnergySetViewModel(UAbilitySystemComponent* ASC)
{
	URsEnergySetViewModel* ViewModel = NewObject<URsEnergySetViewModel>(ASC);
	ViewModel->Initialize();
	return ViewModel;
}

void URsEnergySetViewModel::Initialize()
{
	Super::Initialize();
	
	CachedModel = Cast<UAbilitySystemComponent>(GetOuter());
	if (UAbilitySystemComponent* AbilitySystemComponent = CachedModel.Get())
	{
		SetMaxEnergy(AbilitySystemComponent->GetNumericAttribute(URsEnergySet::GetMaxEnergyAttribute()));
		SetCurrentEnergy(AbilitySystemComponent->GetNumericAttribute(URsEnergySet::GetCurrentEnergyAttribute()));
		SetEnergyRegen(AbilitySystemComponent->GetNumericAttribute(URsEnergySet::GetEnergyRegenAttribute()));
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxEnergyAttribute()).AddUObject(this, &ThisClass::MaxEnergyChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentEnergyAttribute()).AddUObject(this, &ThisClass::CurrentEnergyChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetEnergyRegenAttribute()).AddUObject(this, &ThisClass::EnergyRegenChanged);
	}
}

void URsEnergySetViewModel::Deinitialize()
{
	Super::Deinitialize();
	
	if (UAbilitySystemComponent* AbilitySystemComponent = CachedModel.Get())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetMaxEnergyAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetCurrentEnergyAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsEnergySet::GetEnergyRegenAttribute()).RemoveAll(this);
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
