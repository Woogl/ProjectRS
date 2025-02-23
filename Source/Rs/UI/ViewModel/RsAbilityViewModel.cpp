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
}

float URsAbilityViewModel::GetCooldownDuration() const
{
	return CooldownDuration;
}

float URsAbilityViewModel::GetCooldownRemaining() const
{
	return CooldownRemaining;
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