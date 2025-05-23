// Copyright 2024 Team BH.


#include "RsAbilityViewModel.h"

#include "AbilitySystemComponent.h"
#include "CommonHardwareVisibilityBorder.h"
#include "InputMappingContext.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/Character/RsPlayerCharacter.h"

URsAbilityViewModel* URsAbilityViewModel::CreateRsAbilityViewModel(URsGameplayAbility* Ability)
{
	URsAbilityViewModel* ViewModel = NewObject<URsAbilityViewModel>(Ability);
	ViewModel->Initialize();
	return ViewModel;
}

void URsAbilityViewModel::TryActivateAbility()
{
	if (URsGameplayAbility* Model = CachedModel.Get())
	{
		if (UAbilitySystemComponent* ASC = Model->GetAbilitySystemComponentFromActorInfo())
		{
			ASC->TryActivateAbility(Model->GetCurrentAbilitySpecHandle());
		}
	}
}

void URsAbilityViewModel::Initialize()
{
	Super::Initialize();
	
	CachedModel = Cast<URsGameplayAbility>(GetOuter());
	if (URsGameplayAbility* Model = CachedModel.Get())
	{
		SetCooldownDuration(Model->CooldownDuration);
		SetCurrentRechargeStacks(Model->GetCurrentRechargeStacks());
		SetMaxRechargeStacks(Model->MaxRechargeStacks);
		
		Model->OnRechargeStacksChanged.AddUObject(this, &ThisClass::HandleRechargeStacksChanged);
	}
}

void URsAbilityViewModel::Deinitialize()
{
	Super::Deinitialize();
	
	if (URsGameplayAbility* Model = CachedModel.Get())
	{
		Model->OnRechargeStacksChanged.RemoveAll(this);
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
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownRemainingText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsOnCooldown);
	}
}

void URsAbilityViewModel::SetCurrentRechargeStacks(int32 NewStacks)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentRechargeStacks, NewStacks))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentRechargeStacksText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsOnCooldown);
	}
}

void URsAbilityViewModel::SetMaxRechargeStacks(int32 NewStacks)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxRechargeStacks, NewStacks))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxRechargeStacksText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CurrentRechargeStacks);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsOnCooldown);
	}
}

FText URsAbilityViewModel::GetCooldownRemainingText() const
{
	static const FNumberFormattingOptions Format = FNumberFormattingOptions().SetMinimumFractionalDigits(1).SetMaximumFractionalDigits(1);
	return FText::AsNumber(CooldownRemaining, &Format);
}

float URsAbilityViewModel::GetCooldownPercent() const
{
	if (CooldownDuration == 0.f)
	{
		return 0.f;
	}
	return CooldownRemaining / CooldownDuration;
}

FText URsAbilityViewModel::GetMaxRechargeStacksText() const
{
	return FText::AsNumber(MaxRechargeStacks);
}

FText URsAbilityViewModel::GetCurrentRechargeStacksText() const
{
	return FText::AsNumber(CurrentRechargeStacks);
}

bool URsAbilityViewModel::IsOnCooldown() const
{
	return CooldownRemaining > 0.f;
}

bool URsAbilityViewModel::IsRechargeable() const
{
	return MaxRechargeStacks > 0;;
}

FText URsAbilityViewModel::GetInputKeyText() const
{
	URsGameplayAbility* Model = CachedModel.Get();
	if (Model && Model->ActivationInputAction)
	{
		if (ARsPlayerCharacter* PlayerCharacter = Cast<ARsPlayerCharacter>(Model->GetAvatarCharacter()))
		{
			if (UInputMappingContext* MappingContext = PlayerCharacter->GetDefaultMappingContext())
			{
				for (const FEnhancedActionKeyMapping& Mapping : MappingContext->GetMappings())
				{
					if (Mapping.Action == Model->ActivationInputAction)
					{
						return Mapping.Key.GetDisplayName(false);
					}
				}
			}
		}
	}
	return FText::GetEmpty();
}

UObject* URsAbilityViewModel::GetSkillIcon() const
{
	if (URsGameplayAbility* Model = CachedModel.Get())
	{
		return Model->SkillIcon;
	}
	return nullptr;
}

ESlateVisibility URsAbilityViewModel::GetSkillIconVisibility() const
{
	if (GetSkillIcon())
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Hidden;
}

void URsAbilityViewModel::HandleRechargeStacksChanged(int CurrentStacks)
{
	SetCurrentRechargeStacks(CurrentStacks);
}

void URsAbilityViewModel::Tick(float DeltaTime)
{
	if (URsGameplayAbility* Model = CachedModel.Get())
	{
		SetCooldownRemaining(Model->GetCooldownTimeRemaining());
	}
}

TStatId URsAbilityViewModel::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URsAbilityViewModel, STATGROUP_Tickables);
}
