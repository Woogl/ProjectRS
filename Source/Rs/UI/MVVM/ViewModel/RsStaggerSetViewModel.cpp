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
	
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetMaxStaggerAttribute()).AddUObject(this, &ThisClass::MaxStaggerChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetCurrentStaggerAttribute()).AddUObject(this, &ThisClass::CurrentStaggerChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetStaggerDecayAttribute()).AddUObject(this, &ThisClass::StaggerRegenChanged);
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
	if (URsStaggerSet* StaggerSet = GetModel<ThisClass>())
	{
		return StaggerSet->GetCurrentStagger();
	}
	return 0.f;
}

float URsStaggerSetViewModel::GetMaxStagger() const
{
	if (URsStaggerSet* StaggerSet = GetModel<ThisClass>())
	{
		return StaggerSet->GetMaxStagger();
	}
	return 0.f;
}

float URsStaggerSetViewModel::GetStaggerDecay() const
{
	if (URsStaggerSet* StaggerSet = GetModel<ThisClass>())
	{
		return StaggerSet->GetStaggerDecay();
	}
	return 0.f;
}

float URsStaggerSetViewModel::GetStaggerPercent() const
{
	if (GetMaxStagger() != 0.f)
	{
		return GetCurrentStagger() / GetMaxStagger();
	}
	else
	{
		return 0.f;
	}
}

void URsStaggerSetViewModel::MaxStaggerChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxStagger);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaggerPercent);
}

void URsStaggerSetViewModel::CurrentStaggerChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentStagger);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaggerPercent);
}

void URsStaggerSetViewModel::StaggerRegenChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaggerDecay);
}
