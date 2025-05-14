// Copyright 2024 Team BH.


#include "RsStaggerSetViewModel.h"

#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"

URsStaggerSetViewModel* URsStaggerSetViewModel::CreateStaggerSetViewModel(AActor* Model)
{
	URsStaggerSetViewModel* ViewModel = NewObject<URsStaggerSetViewModel>(Model);
	ViewModel->Initialize();
	return ViewModel;
}

void URsStaggerSetViewModel::Initialize()
{
	Super::Initialize();
	
	const AActor* Model = Cast<AActor>(GetOuter());
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Model))
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
	
	const AActor* Model = Cast<AActor>(GetOuter());
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Model))
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
