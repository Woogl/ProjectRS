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
	if (UAbilitySystemComponent* AbilitySystemComponent = CachedModel.Get())
	{
		SetMaxHealth(AbilitySystemComponent->GetNumericAttribute(URsHealthSet::GetMaxHealthAttribute()));
		SetCurrentHealth(AbilitySystemComponent->GetNumericAttribute(URsHealthSet::GetCurrentHealthAttribute()));
		SetHealthRegen(AbilitySystemComponent->GetNumericAttribute(URsHealthSet::GetHealthRegenAttribute()));
		SetShield(AbilitySystemComponent->GetNumericAttribute(URsHealthSet::GetShieldAttribute()));
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::CurrentHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetHealthRegenAttribute()).AddUObject(this, &ThisClass::HealthRegenChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetShieldAttribute()).AddUObject(this, &ThisClass::ShieldChanged);
	}
}

void URsHealthSetViewModel::Deinitialize()
{
	Super::Deinitialize();
	
	if (UAbilitySystemComponent* AbilitySystemComponent = CachedModel.Get())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetMaxHealthAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetHealthRegenAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetShieldAttribute()).RemoveAll(this);
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
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShieldPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsDead);
	}
}

void URsHealthSetViewModel::SetMaxHealth(float NewMaxHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth))
	{
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
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
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

bool URsHealthSetViewModel::IsDead() const
{
	if (CachedModel.IsValid())
	{
		if (URsBattleLibrary::IsDead(CachedModel->GetAvatarActor()))
		{
			return true;
		}
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
