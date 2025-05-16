// Copyright 2024 Team BH.


#include "RsAbilityViewModel.h"

#include "InputMappingContext.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/Character/RsPlayerCharacter.h"

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
	Super::Initialize();
	
	CachedModel = Cast<URsGameplayAbility>(GetOuter());
	if (CachedModel.IsValid())
	{
		SetCurrentRechargeStacks(CachedModel->GetCurrentRechargeStacks());
		SetMaxRechargeStacks(CachedModel->MaxRechargeStacks);
		SetSkillIcon(CachedModel->SkillIcon);
		
		CachedModel->OnRechargeStacksChanged.AddUObject(this, &ThisClass::HandleRechargeStacksChanged);
	}
}

void URsAbilityViewModel::Deinitialize()
{
	Super::Deinitialize();
	
	if (CachedModel.IsValid())
	{
		CachedModel->OnRechargeStacksChanged.RemoveAll(this);
	}
}

float URsAbilityViewModel::GetCooldownDuration() const
{
	if (CachedModel.IsValid())
	{
		return CachedModel->CooldownDuration.GetValueAtLevel(0);
	}
	return CooldownDuration;
}

float URsAbilityViewModel::GetCooldownRemaining() const
{
	if (CachedModel.IsValid())
	{
		return CachedModel->GetCooldownTimeRemaining();
	}
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

UObject* URsAbilityViewModel::GetSkillIcon() const
{
	return SkillIcon;
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

void URsAbilityViewModel::SetSkillIcon(UObject* NewSkillIcon)
{
	UE_MVVM_SET_PROPERTY_VALUE(SkillIcon, NewSkillIcon);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HasSkillIcon);
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

FText URsAbilityViewModel::GetInputKeyText() const
{
	if (CachedModel.IsValid() && CachedModel->ActivationInputAction)
	{
		if (ARsPlayerCharacter* PlayerCharacter = Cast<ARsPlayerCharacter>(CachedModel->GetAvatarCharacter()))
		{
			if (UInputMappingContext* MappingContext = PlayerCharacter->GetDefaultMappingContext())
			{
				for (const FEnhancedActionKeyMapping& Mapping : MappingContext->GetMappings())
				{
					if (Mapping.Action == CachedModel->ActivationInputAction)
					{
						return Mapping.Key.GetDisplayName(false);
					}
				}
			}
		}
	}
	return FText::GetEmpty();
}

bool URsAbilityViewModel::HasSkillIcon() const
{
	return SkillIcon != nullptr;
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
