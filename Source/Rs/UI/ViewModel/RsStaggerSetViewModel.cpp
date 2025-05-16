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
	if (UAbilitySystemComponent* AbilitySystemComponent = CachedModel.Get())
	{
		SetMaxStagger(AbilitySystemComponent->GetNumericAttribute(URsStaggerSet::GetMaxStaggerAttribute()));
		SetCurrentStagger(AbilitySystemComponent->GetNumericAttribute(URsStaggerSet::GetCurrentStaggerAttribute()));
		SetStaggerRegen(AbilitySystemComponent->GetNumericAttribute(URsStaggerSet::GetStaggerRegenAttribute()));
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetMaxStaggerAttribute()).AddUObject(this, &ThisClass::MaxStaggerChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetCurrentStaggerAttribute()).AddUObject(this, &ThisClass::CurrentStaggerChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetStaggerRegenAttribute()).AddUObject(this, &ThisClass::StaggerRegenChanged);
	}
}

void URsStaggerSetViewModel::Deinitialize()
{
	Super::Deinitialize();
	
	if (UAbilitySystemComponent* AbilitySystemComponent = CachedModel.Get())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetMaxStaggerAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetCurrentStaggerAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetStaggerRegenAttribute()).RemoveAll(this);
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

float URsStaggerSetViewModel::GetStaggerRegen() const
{
	return StaggerRegen;
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

void URsStaggerSetViewModel::SetStaggerRegen(float NewStaggerRegen)
{
	UE_MVVM_SET_PROPERTY_VALUE(StaggerRegen, NewStaggerRegen);
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
	SetStaggerRegen(Data.NewValue);
}
