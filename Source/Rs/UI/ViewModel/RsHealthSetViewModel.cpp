// Copyright 2024 Team BH.


#include "RsHealthSetViewModel.h"

#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"

URsHealthSetViewModel* URsHealthSetViewModel::CreateHealthSetViewModel(AActor* Model)
{
	URsHealthSetViewModel* ViewModel = NewObject<URsHealthSetViewModel>(Model);
	ViewModel->Initialize();
	return ViewModel;
}

void URsHealthSetViewModel::Initialize()
{
	const AActor* Model = Cast<AActor>(GetOuter());
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Model))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::CurrentHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetHealthRegenAttribute()).AddUObject(this, &ThisClass::HealthRegenChanged);

		bool bFound;
		SetMaxHealth(AbilitySystemComponent->GetGameplayAttributeValue(URsHealthSet::GetMaxHealthAttribute(), bFound));
		SetCurrentHealth(AbilitySystemComponent->GetGameplayAttributeValue(URsHealthSet::GetCurrentHealthAttribute(), bFound));
		SetHealthRegen(AbilitySystemComponent->GetGameplayAttributeValue(URsHealthSet::GetHealthRegenAttribute(), bFound));
	}
}

float URsHealthSetViewModel::GetCurrentHealth() const
{
	return CurrentHealth;
}

float URsHealthSetViewModel::GetMaxHealth() const
{
	return MaxHealth;
}

float URsHealthSetViewModel::GetHealthRegen() const
{
	return HealthRegen;
}

void URsHealthSetViewModel::SetCurrentHealth(float NewCurrentHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, NewCurrentHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	}
}

void URsHealthSetViewModel::SetMaxHealth(float NewMaxHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	}
}

void URsHealthSetViewModel::SetHealthRegen(float NewHealthRegen)
{
	UE_MVVM_SET_PROPERTY_VALUE(HealthRegen, NewHealthRegen);
}

float URsHealthSetViewModel::GetHealthPercent() const
{
	if (MaxHealth != 0)
	{
		return CurrentHealth / MaxHealth;
	}
	else
	{
		return 0;
	}
}

void URsHealthSetViewModel::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	SetMaxHealth(Data.NewValue);
}

void URsHealthSetViewModel::CurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	SetCurrentHealth(Data.NewValue);
}

void URsHealthSetViewModel::HealthRegenChanged(const FOnAttributeChangeData& Data)
{
	SetHealthRegen(Data.NewValue);
}
