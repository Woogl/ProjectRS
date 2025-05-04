// Copyright 2024 Team BH.


#include "RsAbilityViewModel.h"

#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"

URsAbilityViewModel* URsAbilityViewModel::CreateRsAbilityViewModel(URsGameplayAbility* Model)
{
	if (Model == nullptr)
	{
		return nullptr;
	}
	
	URsAbilityViewModel* ViewModel = NewObject<URsAbilityViewModel>(Model);
	ViewModel->Initialize();
	return ViewModel;
}

void URsAbilityViewModel::Initialize()
{
	CachedModel = Cast<URsGameplayAbility>(GetOuter());
	if (CachedModel.IsValid())
	{
		SetCurrentRechargeStacks(CachedModel->GetCurrentRechargeStacks());
		SetMaxRechargeStacks(CachedModel->MaxRechargeStacks);
		
		CachedModel->OnRechargeStacksChanged.AddUObject(this, &ThisClass::HandleRechargeStacksChanged);
	}
}

float URsAbilityViewModel::GetCooldownDuration() const
{
	return CooldownDuration;
}

float URsAbilityViewModel::GetCooldownRemaining() const
{
	return CooldownRemaining;
}

int32 URsAbilityViewModel::GetCurrentRechargeStacks() const
{
	return CurrentRechargeStacks;
}

int32 URsAbilityViewModel::GetMaxRechargeStacks() const
{
	return MaxRechargeStacks;
}

void URsAbilityViewModel::SetCooldownDuration(float NewCooldownDuration)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CooldownDuration, NewCooldownDuration))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsOnCooldown);
	}
}

void URsAbilityViewModel::SetCooldownRemaining(float NewCooldownRemaining)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CooldownRemaining, NewCooldownRemaining))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsOnCooldown);
	}
}

void URsAbilityViewModel::SetCurrentRechargeStacks(int32 NewStacks)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentRechargeStacks, NewStacks))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsOnCooldown);
	}
}

void URsAbilityViewModel::SetMaxRechargeStacks(int32 NewStacks)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxRechargeStacks, NewStacks))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsOnCooldown);
	}
}

float URsAbilityViewModel::GetCooldownPercent() const
{
	if (CooldownDuration == 0.f)
	{
		return 0.f;
	}
	return CooldownRemaining / CooldownDuration;
}

bool URsAbilityViewModel::IsOnCooldown() const
{
	return CooldownRemaining > 0.f;
}

bool URsAbilityViewModel::IsRechargeable() const
{
	if (CachedModel.IsValid())
	{
		return CachedModel->MaxRechargeStacks > 0;
	}
	return false;
}

void URsAbilityViewModel::HandleRechargeStacksChanged(int CurrentStacks)
{
	SetCurrentRechargeStacks(CurrentStacks);
}

void URsAbilityViewModel::Tick(float DeltaTime)
{
	if (CachedModel.IsValid())
	{
		float LocalCooldownRemaining;
		float LocalCooldownDuration;
		CachedModel->GetCooldownTimeRemainingAndDuration(FGameplayAbilitySpecHandle(), CachedModel->GetCurrentActorInfo(), LocalCooldownRemaining, LocalCooldownDuration);

		SetCooldownRemaining(LocalCooldownRemaining);
		SetCooldownDuration(LocalCooldownDuration);
	}
}

TStatId URsAbilityViewModel::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URsAbilityViewModel, STATGROUP_Tickables);
}
