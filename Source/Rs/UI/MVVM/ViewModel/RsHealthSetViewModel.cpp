// Copyright 2024 Team BH.


#include "RsHealthSetViewModel.h"

#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/Battle/RsBattleLibrary.h"

URsHealthSetViewModel* URsHealthSetViewModel::CreateHealthSetViewModel(const URsHealthSet* HealthSet)
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
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetBarrierAttribute()).AddUObject(this, &ThisClass::BarrierChanged);
	}
}

void URsHealthSetViewModel::Deinitialize()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetMaxHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetBarrierAttribute()).RemoveAll(this);
	}
	Super::Deinitialize();
}

float URsHealthSetViewModel::GetCurrentHealth() const
{
	if (const URsHealthSet* HealthSet = GetModel<ThisClass>())
	{
		return HealthSet->GetCurrentHealth();
	}
	return 0.f;
}

float URsHealthSetViewModel::GetMaxHealth() const
{
	if (const URsHealthSet* HealthSet = GetModel<ThisClass>())
	{
		return HealthSet->GetMaxHealth();
	}
	return 0.f;
}

float URsHealthSetViewModel::GetBarrier() const
{
	if (const URsHealthSet* HealthSet = GetModel<ThisClass>())
	{
		return HealthSet->GetBarrier();
	}
	return 0.f;
}

float URsHealthSetViewModel::GetHealthPercent() const
{
	if (GetMaxHealth() != 0.f)
	{
		return GetCurrentHealth() / GetMaxHealth();
	}
	else
	{
		return 0.f;
	}
}

float URsHealthSetViewModel::GetBarrierPercent() const
{
	if (GetMaxHealth() != 0.f)
	{
		return GetBarrier() / GetMaxHealth();
	}
	else
	{
		return 0.f;
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
	if (ASC.IsValid())
	{
		return URsBattleLibrary::IsDead(ASC->GetAvatarActor());
	}
	return false;
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

	float HealthPercent = GetHealthPercent();
	if (HealthPercent >= 0.75f)
	{
		return AliveColor;
	}
	if (HealthPercent <= 0.25f)
	{
		return DeadColor;
	}
	
	float Alpha = (HealthPercent - 0.25f) / (0.75f - 0.25f);
	return FLinearColor::LerpUsingHSV(DeadColor, AliveColor, Alpha);
}

void URsHealthSetViewModel::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxHealth);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxHealthText);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetBarrierPercent);
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

void URsHealthSetViewModel::BarrierChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetBarrier);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetBarrierPercent);
}
