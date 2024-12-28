// Copyright 2024 Team BH.


#include "StaggerSetViewModel.h"

#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/StaggerSet.h"

UStaggerSetViewModel* UStaggerSetViewModel::CreateStaggerSetViewModel(AActor* Model)
{
	UStaggerSetViewModel* ViewModel = NewObject<UStaggerSetViewModel>(Model);
	ViewModel->Initialize();
	return ViewModel;
}

void UStaggerSetViewModel::Initialize()
{
	const AActor* Model = Cast<AActor>(GetOuter());
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Model))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStaggerSet::GetMaxStaggerAttribute()).AddUObject(this, &ThisClass::MaxStaggerChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStaggerSet::GetCurrentStaggerAttribute()).AddUObject(this, &ThisClass::CurrentStaggerChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStaggerSet::GetStaggerRegenAttribute()).AddUObject(this, &ThisClass::StaggerRegenChanged);

		bool bFound;
		SetMaxStagger(AbilitySystemComponent->GetGameplayAttributeValue(UStaggerSet::GetMaxStaggerAttribute(), bFound));
		SetCurrentStagger(AbilitySystemComponent->GetGameplayAttributeValue(UStaggerSet::GetCurrentStaggerAttribute(), bFound));
		SetStaggerRegen(AbilitySystemComponent->GetGameplayAttributeValue(UStaggerSet::GetStaggerRegenAttribute(), bFound));
	}
}

float UStaggerSetViewModel::GetCurrentStagger() const
{
	return CurrentStagger;
}

float UStaggerSetViewModel::GetMaxStagger() const
{
	return MaxStagger;
}

float UStaggerSetViewModel::GetStaggerRegen() const
{
	return StaggerRegen;
}

void UStaggerSetViewModel::SetCurrentStagger(float NewCurrentStagger)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentStagger, NewCurrentStagger))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaggerPercent);
	}
}

void UStaggerSetViewModel::SetMaxStagger(float NewMaxStagger)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxStagger, NewMaxStagger))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStaggerPercent);
	}
}

void UStaggerSetViewModel::SetStaggerRegen(float NewStaggerRegen)
{
	UE_MVVM_SET_PROPERTY_VALUE(StaggerRegen, NewStaggerRegen);
}

float UStaggerSetViewModel::GetStaggerPercent() const
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

void UStaggerSetViewModel::MaxStaggerChanged(const FOnAttributeChangeData& Data)
{
	SetMaxStagger(Data.NewValue);
}

void UStaggerSetViewModel::CurrentStaggerChanged(const FOnAttributeChangeData& Data)
{
	SetCurrentStagger(Data.NewValue);
}

void UStaggerSetViewModel::StaggerRegenChanged(const FOnAttributeChangeData& Data)
{
	SetStaggerRegen(Data.NewValue);
}
