// Copyright 2024 Team BH.


#include "HealthSetViewModel.h"

#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/HealthSet.h"

UHealthSetViewModel* UHealthSetViewModel::CreateHealthSetViewModel(AActor* Model)
{
	UHealthSetViewModel* ViewModel = NewObject<UHealthSetViewModel>(Model);
	ViewModel->Initialize();
	return ViewModel;
}

void UHealthSetViewModel::Initialize()
{
	const AActor* Model = Cast<AActor>(GetOuter());
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Model))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthSet::GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::CurrentHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthSet::GetHealthRegenAttribute()).AddUObject(this, &ThisClass::HealthRegenChanged);

		bool bFound;
		SetMaxHealth(AbilitySystemComponent->GetGameplayAttributeValue(UHealthSet::GetMaxHealthAttribute(), bFound));
		SetCurrentHealth(AbilitySystemComponent->GetGameplayAttributeValue(UHealthSet::GetCurrentHealthAttribute(), bFound));
		SetHealthRegen(AbilitySystemComponent->GetGameplayAttributeValue(UHealthSet::GetHealthRegenAttribute(), bFound));
	}
}

float UHealthSetViewModel::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthSetViewModel::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthSetViewModel::GetHealthRegen() const
{
	return HealthRegen;
}

void UHealthSetViewModel::SetCurrentHealth(float NewCurrentHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, NewCurrentHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	}
}

void UHealthSetViewModel::SetMaxHealth(float NewMaxHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	}
}

void UHealthSetViewModel::SetHealthRegen(float NewHealthRegen)
{
	UE_MVVM_SET_PROPERTY_VALUE(HealthRegen, NewHealthRegen);
}

float UHealthSetViewModel::GetHealthPercent() const
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

void UHealthSetViewModel::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	SetMaxHealth(Data.NewValue);
}

void UHealthSetViewModel::CurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	SetCurrentHealth(Data.NewValue);
}

void UHealthSetViewModel::HealthRegenChanged(const FOnAttributeChangeData& Data)
{
	SetHealthRegen(Data.NewValue);
}
