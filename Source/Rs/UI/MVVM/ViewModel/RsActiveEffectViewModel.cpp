// Copyright 2024 Team BH.


#include "RsActiveEffectViewModel.h"

#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/EffectComponent/RsUIDataEffectComponent.h"

URsActiveEffectViewModel* URsActiveEffectViewModel::CreateRsActiveEffectViewModel(FActiveGameplayEffectHandle EffectHandle)
{
	UAbilitySystemComponent* ASC = EffectHandle.GetOwningAbilitySystemComponent();
	const FActiveGameplayEffect* ActiveEffect = ASC->GetActiveGameplayEffect(EffectHandle);
	if (!ASC || !ActiveEffect)
	{
		return nullptr;
	}
	
	// Active effect view model must have UI Data.
	if (!ActiveEffect->Spec.Def->FindComponent<URsUIDataEffectComponent>())
	{
		return nullptr;
	}
	
	URsActiveEffectViewModel* ViewModel = NewObject<URsActiveEffectViewModel>(ASC);
	ViewModel->EffectHandle = EffectHandle;
	ViewModel->Initialize();
	return ViewModel;
}

void URsActiveEffectViewModel::Initialize()
{
	Super::Initialize();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (const FActiveGameplayEffect* ActiveEffect = GetActiveEffect())
		{
			SetDuration(ActiveEffect->GetDuration());
		}
		
		ASC->OnGameplayEffectTimeChangeDelegate(EffectHandle)->AddWeakLambda(this, [this](FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration)
		{
			HandleEffectTimeChange(EffectHandle, NewStartTime, NewDuration);
		});
	}
}

void URsActiveEffectViewModel::Deinitialize()
{
	// unsafe to remove delegate bind, because ActiveEffect which has binded delegate might be already deleted in ASC
	Super::Deinitialize();
}

void URsActiveEffectViewModel::HandleEffectTimeChange(FActiveGameplayEffectHandle InEffectHandle, float NewStartTime, float NewDuration)
{
	if (EffectHandle != InEffectHandle)
	{
		return;
	}
	
	const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
	SetRemainingTime(FMath::Max(0.f, NewDuration - FMath::Max(0.f, Now - NewStartTime)));
	SetDuration(NewDuration);
}

UObject* URsActiveEffectViewModel::GetIcon() const
{
	if (const URsUIDataEffectComponent* UIData = GetUIData())
	{
		return UIData->GetIcon();
	}
	return nullptr;
}

FText URsActiveEffectViewModel::GetDescription() const
{
	if (const URsUIDataEffectComponent* UIData = GetUIData())
	{
		return UIData->GetDescription();
	}
	return FText::GetEmpty();
}

int32 URsActiveEffectViewModel::GetPriority() const
{
	if (const URsUIDataEffectComponent* UIData = GetUIData())
	{
		return UIData->GetPriority();
	}
	return 0;
}

float URsActiveEffectViewModel::GetDuration() const
{
	return Duration;
}

void URsActiveEffectViewModel::SetDuration(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Duration, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDurationText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetTimeProgress);
	}
}

int32 URsActiveEffectViewModel::GetStacks() const
{
	if (const FActiveGameplayEffect* ActiveEffect = GetActiveEffect())
	{
		return ActiveEffect->ClientCachedStackCount;
	}
	return 0;
}

void URsActiveEffectViewModel::SetStacks(int32 Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Stacks, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStacksText);
	}
}

float URsActiveEffectViewModel::GetRemainingTime() const
{
	return RemainingTime;
}

void URsActiveEffectViewModel::SetRemainingTime(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(RemainingTime, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetRemainingTimeText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetTimeProgress);
	}
}

int32 URsActiveEffectViewModel::GetMaxStacks() const
{
	return MaxStacks;
}

void URsActiveEffectViewModel::SetMaxStacks(int32 Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxStacks, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxStacksText);
	}
}

FText URsActiveEffectViewModel::GetStacksText() const
{
	const int32 Stack = GetStacks();
	return Stack > 1 ? FText::AsNumber(Stack) : FText::GetEmpty();
}

FText URsActiveEffectViewModel::GetMaxStacksText() const
{
	return FText::AsNumber(MaxStacks);
}

FText URsActiveEffectViewModel::GetDurationText() const
{
	return FText::AsNumber(Duration);
}

FText URsActiveEffectViewModel::GetRemainingTimeText() const
{
	return FText::AsNumber(RemainingTime);
}

float URsActiveEffectViewModel::GetTimeProgress() const
{
	if (Duration == 0)
	{
		return 0.f;
	}
	return RemainingTime / Duration;
}

void URsActiveEffectViewModel::Tick(float DeltaTime)
{
	if (const FActiveGameplayEffect* ActiveEffect = GetActiveEffect())
	{
		float WorldTime = GetWorld()->GetTimeSeconds();
		SetRemainingTime(ActiveEffect->GetTimeRemaining(WorldTime));
	}
}

TStatId URsActiveEffectViewModel::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URsAbilityViewModel, STATGROUP_Tickables);
}

const FActiveGameplayEffect* URsActiveEffectViewModel::GetActiveEffect() const
{
	if (const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		return ASC->GetActiveGameplayEffect(EffectHandle);
	}
	return nullptr;
}

const URsUIDataEffectComponent* URsActiveEffectViewModel::GetUIData() const
{
	if (const FActiveGameplayEffect* ActiveEffect = GetActiveEffect())
	{
		return ActiveEffect->Spec.Def->FindComponent<URsUIDataEffectComponent>();
	}
	return nullptr;
}

UAbilitySystemComponent* URsActiveEffectViewModel::GetAbilitySystemComponent() const
{
	return EffectHandle.GetOwningAbilitySystemComponent();
}
