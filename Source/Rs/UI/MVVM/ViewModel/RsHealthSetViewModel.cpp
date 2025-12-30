// Copyright 2024 Team BH.


#include "RsHealthSetViewModel.h"

#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/UI/MVVM/RsMVVMConversionLibrary.h"

URsHealthSetViewModel* URsHealthSetViewModel::CreateHealthSetViewModel(const URsHealthSet* HealthSet)
{
	return CreateViewModel<URsHealthSetViewModel>(HealthSet);
}

void URsHealthSetViewModel::Initialize()
{
	Super::Initialize();
	
	// Initial value set
	if (const URsHealthSet* HealthSet = GetModel<ThisClass>())
	{
		SetMaxHealth(HealthSet->GetMaxHealth());
		SetCurrentHealth(HealthSet->GetCurrentHealth());
		SetBarrier(HealthSet->GetBarrier());
	}
}

void URsHealthSetViewModel::HandleAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (Data.Attribute == URsHealthSet::GetMaxHealthAttribute())
	{
		SetMaxHealth(Data.NewValue);
	}
	else if (Data.Attribute == URsHealthSet::GetCurrentHealthAttribute())
	{
		SetCurrentHealth(Data.NewValue);
	}
	else if (Data.Attribute == URsHealthSet::GetBarrierAttribute())
	{
		SetBarrier(Data.NewValue);
	}
	
	Super::HandleAttributeChanged(Data);
}

float URsHealthSetViewModel::GetCurrentHealth() const
{
	return CurrentHealth;
}

void URsHealthSetViewModel::SetCurrentHealth(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentHealthText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsDead);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthBarColor);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPortraitColor);
	}
}

float URsHealthSetViewModel::GetMaxHealth() const
{
	return MaxHealth;
}

void URsHealthSetViewModel::SetMaxHealth(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxHealthText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetBarrierPercent);
	}
}

FText URsHealthSetViewModel::GetCurrentHealthText() const
{
	return URsMVVMConversionLibrary::FloatToText(CurrentHealth, 0);
}

FText URsHealthSetViewModel::GetMaxHealthText() const
{
	return URsMVVMConversionLibrary::FloatToText(MaxHealth, 0);
}

float URsHealthSetViewModel::GetHealthPercent() const
{
	if (MaxHealth != 0.f)
	{
		return CurrentHealth / MaxHealth;
	}
	else
	{
		return 0.f;
	}
}

float URsHealthSetViewModel::GetBarrier() const
{
	return Barrier;
}

void URsHealthSetViewModel::SetBarrier(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Barrier, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetBarrierPercent);
	}
}

float URsHealthSetViewModel::GetBarrierPercent() const
{
	if (MaxHealth != 0.f)
	{
		return Barrier / MaxHealth;
	}
	else
	{
		return 0.f;
	}
}

bool URsHealthSetViewModel::IsDead() const
{
	if (ASC.IsValid())
	{
		return URsBattleLibrary::IsDead(ASC->GetAvatarActor());
	}
	return false;
}

FLinearColor URsHealthSetViewModel::GetPortraitColor() const
{
	if (IsDead() == true)
	{
		return FLinearColor(0.15f, 0.15f, 0.15f);	
	}
	return FLinearColor::White;
}

FLinearColor URsHealthSetViewModel::GetHealthBarColor() const
{
	return FLinearColor::Green;
}
