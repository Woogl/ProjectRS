// Copyright 2024 Team BH.


#include "RsStaggerSetViewModel.h"

#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"

URsStaggerSetViewModel* URsStaggerSetViewModel::CreateStaggerSetViewModel(const URsStaggerSet* StaggerSet)
{
	return CreateViewModel<URsStaggerSetViewModel>(StaggerSet);
}

void URsStaggerSetViewModel::Initialize()
{
	Super::Initialize();

	// Initial value set
	if (URsStaggerSet* StaggerSet = GetModel<ThisClass>())
	{
		SetMaxStagger(StaggerSet->GetMaxStagger());
		SetCurrentStagger(StaggerSet->GetCurrentStagger());
		SetStaggerDecay(StaggerSet->GetStaggerDecay());
	}
}

void URsStaggerSetViewModel::HandleAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (Data.Attribute == URsStaggerSet::GetMaxStaggerAttribute())
	{
		SetMaxStagger(Data.NewValue);
	}
	else if (Data.Attribute == URsStaggerSet::GetCurrentStaggerAttribute())
	{
		SetCurrentStagger(Data.NewValue);
	}
	else if (Data.Attribute == URsStaggerSet::GetStaggerDecayAttribute())
	{
		SetStaggerDecay(Data.NewValue);
	}
	
	Super::HandleAttributeChanged(Data);
}

float URsStaggerSetViewModel::GetCurrentStagger() const
{
	return CurrentStagger;
}

void URsStaggerSetViewModel::SetCurrentStagger(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentStagger, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaggerPercent);
	}
}

float URsStaggerSetViewModel::GetMaxStagger() const
{
	return MaxStagger;
}

void URsStaggerSetViewModel::SetMaxStagger(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxStagger, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaggerPercent);
	}
}

float URsStaggerSetViewModel::GetStaggerDecay() const
{
	return StaggerDecay;
}

void URsStaggerSetViewModel::SetStaggerDecay(float Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(StaggerDecay, Value);
}

float URsStaggerSetViewModel::GetStaggerPercent() const
{
	if (MaxStagger != 0.f)
	{
		return CurrentStagger / MaxStagger;
	}
	else
	{
		return 0.f;
	}
}
