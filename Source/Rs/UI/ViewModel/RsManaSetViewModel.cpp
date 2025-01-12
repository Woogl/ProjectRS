// Copyright 2024 Team BH.


#include "RsManaSetViewModel.h"

#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Attributes/RsManaSet.h"

URsManaSetViewModel* URsManaSetViewModel::CreateManaSetViewModel(AActor* Model)
{
	URsManaSetViewModel* ViewModel = NewObject<URsManaSetViewModel>(Model);
	ViewModel->Initialize();
	return ViewModel;
}

void URsManaSetViewModel::Initialize()
{
	const AActor* Model = Cast<AActor>(GetOuter());
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Model))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsManaSet::GetMaxManaAttribute()).AddUObject(this, &ThisClass::MaxManaChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsManaSet::GetCurrentManaAttribute()).AddUObject(this, &ThisClass::CurrentManaChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsManaSet::GetManaRegenAttribute()).AddUObject(this, &ThisClass::ManaRegenChanged);

		bool bFound;
		SetMaxMana(AbilitySystemComponent->GetGameplayAttributeValue(URsManaSet::GetMaxManaAttribute(), bFound));
		SetCurrentMana(AbilitySystemComponent->GetGameplayAttributeValue(URsManaSet::GetCurrentManaAttribute(), bFound));
		SetManaRegen(AbilitySystemComponent->GetGameplayAttributeValue(URsManaSet::GetManaRegenAttribute(), bFound));
	}
}

float URsManaSetViewModel::GetCurrentMana() const
{
	return CurrentMana;
}

float URsManaSetViewModel::GetMaxMana() const
{
	return CurrentMana;
}

float URsManaSetViewModel::GetManaRegen() const
{
	return ManaRegen;
}

void URsManaSetViewModel::SetCurrentMana(float NewCurrentMana)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentMana, NewCurrentMana))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetManaPercent);
	}
}

void URsManaSetViewModel::SetMaxMana(float NewMaxMana)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxMana, NewMaxMana))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetManaPercent);
	}
}

void URsManaSetViewModel::SetManaRegen(float NewManaRegen)
{
	UE_MVVM_SET_PROPERTY_VALUE(ManaRegen, NewManaRegen);
}

float URsManaSetViewModel::GetManaPercent() const
{
	if (MaxMana != 0)
	{
		return CurrentMana / MaxMana;
	}
	else
	{
		return 0;
	}
}

void URsManaSetViewModel::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	SetMaxMana(Data.NewValue);
}

void URsManaSetViewModel::CurrentManaChanged(const FOnAttributeChangeData& Data)
{
	SetCurrentMana(Data.NewValue);
}

void URsManaSetViewModel::ManaRegenChanged(const FOnAttributeChangeData& Data)
{
	SetManaRegen(Data.NewValue);
}
