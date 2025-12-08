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
	
	// Data bindings
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetMaxStaggerAttribute()).AddUObject(this, &ThisClass::MaxStaggerChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetCurrentStaggerAttribute()).AddUObject(this, &ThisClass::CurrentStaggerChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetStaggerDecayAttribute()).AddUObject(this, &ThisClass::StaggerDecayChanged);
	}

	// Initial value set
	if (URsStaggerSet* StaggerSet = GetModel<ThisClass>())
	{
		SetMaxStagger(StaggerSet->GetMaxStagger());
		SetCurrentStagger(StaggerSet->GetCurrentStagger());
		SetStaggerDecay(StaggerSet->GetStaggerDecay());
	}
}

void URsStaggerSetViewModel::Deinitialize()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetMaxStaggerAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetCurrentStaggerAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetStaggerDecayAttribute()).RemoveAll(this);
	}
	
	Super::Deinitialize();
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

void URsStaggerSetViewModel::MaxStaggerChanged(const FOnAttributeChangeData& Data)
{
	SetMaxStagger(Data.NewValue);
}

void URsStaggerSetViewModel::CurrentStaggerChanged(const FOnAttributeChangeData& Data)
{
	SetCurrentStagger(Data.NewValue);
}

void URsStaggerSetViewModel::StaggerDecayChanged(const FOnAttributeChangeData& Data)
{
	SetStaggerDecay(Data.NewValue);
}

