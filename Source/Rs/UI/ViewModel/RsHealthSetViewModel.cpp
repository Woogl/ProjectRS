// Copyright 2024 Team BH.


#include "RsHealthSetViewModel.h"

#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/Battle/RsBattleLibrary.h"

URsHealthSetViewModel* URsHealthSetViewModel::CreateHealthSetViewModel(UAbilitySystemComponent* ASC)
{
	URsHealthSetViewModel* ViewModel = NewObject<URsHealthSetViewModel>(ASC);
	ViewModel->Initialize();
	return ViewModel;
}

void URsHealthSetViewModel::Initialize()
{
	Super::Initialize();
	
	CachedModel = Cast<UAbilitySystemComponent>(GetOuter());
	if (UAbilitySystemComponent* Model = CachedModel.Get())
	{
		SetMaxHealth(Model->GetNumericAttribute(URsHealthSet::GetMaxHealthAttribute()));
		SetCurrentHealth(Model->GetNumericAttribute(URsHealthSet::GetCurrentHealthAttribute()));
		SetHealthRegen(Model->GetNumericAttribute(URsHealthSet::GetHealthRegenAttribute()));
		SetShield(Model->GetNumericAttribute(URsHealthSet::GetShieldAttribute()));
		
		Model->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);
		Model->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::CurrentHealthChanged);
		Model->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetHealthRegenAttribute()).AddUObject(this, &ThisClass::HealthRegenChanged);
		Model->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetShieldAttribute()).AddUObject(this, &ThisClass::ShieldChanged);
	}
}

void URsHealthSetViewModel::Deinitialize()
{
	Super::Deinitialize();
	
	if (UAbilitySystemComponent* Model = CachedModel.Get())
	{
		Model->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetMaxHealthAttribute()).RemoveAll(this);
		Model->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).RemoveAll(this);
		Model->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetHealthRegenAttribute()).RemoveAll(this);
		Model->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetShieldAttribute()).RemoveAll(this);
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

float URsHealthSetViewModel::GetShield() const
{
	return Shield;
}

void URsHealthSetViewModel::SetCurrentHealth(float NewCurrentHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, NewCurrentHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentHealthText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsDead);
	}
}

void URsHealthSetViewModel::SetMaxHealth(float NewMaxHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxHealthText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShieldPercent);
	}
}

void URsHealthSetViewModel::SetHealthRegen(float NewHealthRegen)
{
	UE_MVVM_SET_PROPERTY_VALUE(HealthRegen, NewHealthRegen);
}

void URsHealthSetViewModel::SetShield(float NewShield)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Shield, NewShield))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShieldPercent);
	}
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

float URsHealthSetViewModel::GetShieldPercent() const
{
	if (MaxHealth != 0)
	{
		return Shield / MaxHealth;
	}
	else
	{
		return 0;
	}
}

FText URsHealthSetViewModel::GetCurrentHealthText() const
{
	static const FNumberFormattingOptions Format = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	return FText::AsNumber(CurrentHealth, &Format);
}

FText URsHealthSetViewModel::GetMaxHealthText() const
{
	static const FNumberFormattingOptions Format = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	return FText::AsNumber(MaxHealth, &Format);
}

bool URsHealthSetViewModel::IsDead() const
{
	if (UAbilitySystemComponent* Model = CachedModel.Get())
	{
		return URsBattleLibrary::IsDead(Model->GetAvatarActor());
	}
	return false;
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

void URsHealthSetViewModel::ShieldChanged(const FOnAttributeChangeData& Data)
{
	SetShield(Data.NewValue);
}
