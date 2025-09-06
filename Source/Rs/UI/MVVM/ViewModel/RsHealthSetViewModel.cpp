// Copyright 2024 Team BH.


#include "RsHealthSetViewModel.h"

#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/Battle/RsBattleLibrary.h"

URsHealthSetViewModel* URsHealthSetViewModel::CreateHealthSetViewModel(URsHealthSet* HealthSet)
{
	return CreateViewModel<URsHealthSetViewModel>(HealthSet);
}

void URsHealthSetViewModel::Initialize()
{
	Super::Initialize();

	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::CurrentHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetHealthRegenAttribute()).AddUObject(this, &ThisClass::HealthRegenChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetShieldAttribute()).AddUObject(this, &ThisClass::ShieldChanged);
	}
}

void URsHealthSetViewModel::Deinitialize()
{
	Super::Deinitialize();

	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetMaxHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetHealthRegenAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetShieldAttribute()).RemoveAll(this);
	}
}

float URsHealthSetViewModel::GetCurrentHealth() const
{
	if (const URsHealthSet* HealthSet = GetModel<URsHealthSet>())
	{
		return HealthSet->GetCurrentHealth();
	}
	return 0.f;
}

float URsHealthSetViewModel::GetMaxHealth() const
{
	if (const URsHealthSet* HealthSet = GetModel<URsHealthSet>())
	{
		return HealthSet->GetMaxHealth();
	}
	return 0.f;
}

float URsHealthSetViewModel::GetHealthRegen() const
{
	if (const URsHealthSet* HealthSet = GetModel<URsHealthSet>())
	{
		return HealthSet->GetHealthRegen();
	}
	return 0.f;
}

float URsHealthSetViewModel::GetShield() const
{
	if (const URsHealthSet* HealthSet = GetModel<URsHealthSet>())
	{
		return HealthSet->GetShield();
	}
	return 0.f;
}

float URsHealthSetViewModel::GetHealthPercent() const
{
	if (GetMaxHealth() != 0)
	{
		return GetCurrentHealth() / GetMaxHealth();
	}
	else
	{
		return 0;
	}
}

float URsHealthSetViewModel::GetShieldPercent() const
{
	if (GetMaxHealth() != 0)
	{
		return GetShield() / GetMaxHealth();
	}
	else
	{
		return 0;
	}
}

FText URsHealthSetViewModel::GetCurrentHealthText() const
{
	static const FNumberFormattingOptions Format = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	return FText::AsNumber(GetCurrentHealth(), &Format);
}

FText URsHealthSetViewModel::GetMaxHealthText() const
{
	static const FNumberFormattingOptions Format = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	return FText::AsNumber(GetMaxHealth(), &Format);
}

bool URsHealthSetViewModel::IsDead() const
{
	return URsBattleLibrary::IsDead(ASC->GetAvatarActor());
}

FLinearColor URsHealthSetViewModel::GetColorByDeathState() const
{
	if (IsDead() == true)
	{
		return FLinearColor(0.15f, 0.15f, 0.15f);	
	}
	return FLinearColor::White;
}

FLinearColor URsHealthSetViewModel::GetColorByHealthPercent() const
{
	static const FLinearColor DeadColor = FLinearColor::Red;
	static const FLinearColor AliveColor = FLinearColor::Green;
	return FMath::Lerp(DeadColor, AliveColor, GetHealthPercent());
}

void URsHealthSetViewModel::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxHealth);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxHealthText);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShieldPercent);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetColorByHealthPercent);
}

void URsHealthSetViewModel::CurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentHealth);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentHealthText);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsDead);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetColorByDeathState);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetColorByHealthPercent);
}

void URsHealthSetViewModel::HealthRegenChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthRegen);
}

void URsHealthSetViewModel::ShieldChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShield);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShieldPercent);
}
