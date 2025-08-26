// Copyright 2024 Team BH.


#include "RsHealthSetViewModel.h"

#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/Battle/RsBattleLibrary.h"

URsHealthSetViewModel* URsHealthSetViewModel::CreateHealthSetViewModel(UAbilitySystemComponent* ASC)
{
	URsHealthSetViewModel* ViewModel = NewObject<URsHealthSetViewModel>(ASC);
	ViewModel->SetModel(ASC);
	ViewModel->Initialize();
	return ViewModel;
}

void URsHealthSetViewModel::Initialize()
{
	Super::Initialize();
	
	if (UAbilitySystemComponent* ASC = GetModel<UAbilitySystemComponent>())
	{
		SetMaxHealth(ASC->GetNumericAttribute(URsHealthSet::GetMaxHealthAttribute()));
		SetCurrentHealth(ASC->GetNumericAttribute(URsHealthSet::GetCurrentHealthAttribute()));
		SetHealthRegen(ASC->GetNumericAttribute(URsHealthSet::GetHealthRegenAttribute()));
		SetShield(ASC->GetNumericAttribute(URsHealthSet::GetShieldAttribute()));
		
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::CurrentHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetHealthRegenAttribute()).AddUObject(this, &ThisClass::HealthRegenChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetShieldAttribute()).AddUObject(this, &ThisClass::ShieldChanged);
	}
}

void URsHealthSetViewModel::Deinitialize()
{
	Super::Deinitialize();
	
	if (UAbilitySystemComponent* ASC = GetModel<UAbilitySystemComponent>())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetMaxHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetHealthRegenAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetShieldAttribute()).RemoveAll(this);
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
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetColorByHealth);
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
	if (UAbilitySystemComponent* ASC = GetModel<UAbilitySystemComponent>())
	{
		return URsBattleLibrary::IsDead(ASC->GetAvatarActor());
	}
	return false;
}

FLinearColor URsHealthSetViewModel::GetColorByHealth() const
{
	if (IsDead() == true)
	{
		return FLinearColor(0.15f, 0.15f, 0.15f);	
	}
	return FLinearColor::White;
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
