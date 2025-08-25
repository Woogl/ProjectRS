// Copyright 2024 Team BH.


#include "RsStaggerSetViewModel.h"

#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"

URsStaggerSetViewModel* URsStaggerSetViewModel::CreateStaggerSetViewModel(UAbilitySystemComponent* ASC)
{
	URsStaggerSetViewModel* ViewModel = NewObject<URsStaggerSetViewModel>(ASC);
	ViewModel->Initialize();
	return ViewModel;
}

void URsStaggerSetViewModel::Initialize()
{
	Super::Initialize();
	
	CachedModel = Cast<UAbilitySystemComponent>(GetOuter());
	if (UAbilitySystemComponent* Model = CachedModel.Get())
	{
		SetMaxStagger(Model->GetNumericAttribute(URsStaggerSet::GetMaxStaggerAttribute()));
		SetCurrentStagger(Model->GetNumericAttribute(URsStaggerSet::GetCurrentStaggerAttribute()));
		SetStaggerDecay(Model->GetNumericAttribute(URsStaggerSet::GetStaggerDecayAttribute()));
		
		Model->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetMaxStaggerAttribute()).AddUObject(this, &ThisClass::MaxStaggerChanged);
		Model->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetCurrentStaggerAttribute()).AddUObject(this, &ThisClass::CurrentStaggerChanged);
		Model->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetStaggerDecayAttribute()).AddUObject(this, &ThisClass::StaggerRegenChanged);
	}
}

void URsStaggerSetViewModel::Deinitialize()
{
	Super::Deinitialize();
	
	if (UAbilitySystemComponent* Model = CachedModel.Get())
	{
		Model->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetMaxStaggerAttribute()).RemoveAll(this);
		Model->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetCurrentStaggerAttribute()).RemoveAll(this);
		Model->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetStaggerDecayAttribute()).RemoveAll(this);
	}
}

float URsStaggerSetViewModel::GetCurrentStagger() const
{
	return CurrentStagger;
}

float URsStaggerSetViewModel::GetMaxStagger() const
{
	return MaxStagger;
}

float URsStaggerSetViewModel::GetStaggerDecay() const
{
	return StaggerDecay;
}

void URsStaggerSetViewModel::SetCurrentStagger(float NewCurrentStagger)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentStagger, NewCurrentStagger))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaggerPercent);
	}
}

void URsStaggerSetViewModel::SetMaxStagger(float NewMaxStagger)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxStagger, NewMaxStagger))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaggerPercent);
	}
}

void URsStaggerSetViewModel::SetStaggerDecay(float NewStaggerDecay)
{
	UE_MVVM_SET_PROPERTY_VALUE(StaggerDecay, NewStaggerDecay);
}

float URsStaggerSetViewModel::GetStaggerPercent() const
{
	if (MaxStagger != 0)
	{
		return CurrentStagger / MaxStagger;
	}
	else
	{
		return 0;
	}
}

void URsStaggerSetViewModel::MaxStaggerChanged(const FOnAttributeChangeData& Data)
{
	SetMaxStagger(Data.NewValue);
}

void URsStaggerSetViewModel::CurrentStaggerChanged(const FOnAttributeChangeData& Data)
{
	SetCurrentStagger(Data.NewValue);
}

void URsStaggerSetViewModel::StaggerRegenChanged(const FOnAttributeChangeData& Data)
{
	SetStaggerDecay(Data.NewValue);
}
