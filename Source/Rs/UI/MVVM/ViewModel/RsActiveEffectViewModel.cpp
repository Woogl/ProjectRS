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
	const URsUIDataEffectComponent* UIData = ActiveEffect->Spec.Def->FindComponent<URsUIDataEffectComponent>();
	if (!UIData)
	{
		return nullptr;
	}
	
	URsActiveEffectViewModel* ViewModel = NewObject<URsActiveEffectViewModel>(ASC);
	ViewModel->CachedASC = Cast<URsAbilitySystemComponent>(ASC);
	ViewModel->CachedUIData = UIData;
	ViewModel->CachedEffectHandle = EffectHandle;
	ViewModel->Initialize();
	return ViewModel;
}

int32 URsActiveEffectViewModel::GetStacks() const
{
	const FActiveGameplayEffect* ActiveEffect = GetActiveEffect();
	return ActiveEffect ? ActiveEffect->ClientCachedStackCount : 0;
}

void URsActiveEffectViewModel::SetStacks(int32 Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Stacks, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStacksText);
	}
}

FText URsActiveEffectViewModel::GetStacksText() const
{
	int32 Stack = GetStacks();
	return Stack > 1 ? FText::AsNumber(Stack) : FText::GetEmpty();
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

FText URsActiveEffectViewModel::GetMaxStacksText() const
{
	return FText::AsNumber(MaxStacks);
}

int32 URsActiveEffectViewModel::GetPriority() const
{
	return CachedUIData.Get() ? CachedUIData->GetPriority() : 0;
}

UObject* URsActiveEffectViewModel::GetIcon() const
{
	return CachedUIData.Get() ? CachedUIData->GetIcon() : nullptr;
}

FText URsActiveEffectViewModel::GetDescription() const
{
	return CachedUIData.Get() ? CachedUIData->GetDescription() : FText();
}

void URsActiveEffectViewModel::SetDuration(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Duration, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDurationText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
	}
}

float URsActiveEffectViewModel::GetDuration() const
{
	return Duration;
}

FText URsActiveEffectViewModel::GetDurationText() const
{
	return FText::AsNumber(Duration);
}

void URsActiveEffectViewModel::SetRemainingTime(float Value)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(RemainingTime, Value))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetRemainingTimeText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
	}
}

float URsActiveEffectViewModel::GetRemainingTime() const
{
	return RemainingTime;
}

FText URsActiveEffectViewModel::GetRemainingTimeText() const
{
	return FText::AsNumber(RemainingTime);
}

float URsActiveEffectViewModel::GetEffectProgress() const
{
	if (Duration == 0)
	{
		return 0.f;
	}
	return RemainingTime / Duration;
}

void URsActiveEffectViewModel::HandleEffectTimeChange(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration)
{
	if (CachedEffectHandle != EffectHandle)
	{
		return;
	}
	
	const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
	SetRemainingTime(FMath::Max(0.f, NewDuration - FMath::Max(0.f, Now - NewStartTime)));
	SetDuration(NewDuration);
}

void URsActiveEffectViewModel::HandleEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	OnViewModelDisabled.Execute(this);
}

const URsUIDataEffectComponent* URsActiveEffectViewModel::GetUIData() const
{
	return CachedUIData.Get();
}

const FActiveGameplayEffect* URsActiveEffectViewModel::GetActiveEffect() const
{
	return CachedASC.Get() ? CachedASC->GetActiveGameplayEffect(CachedEffectHandle) : nullptr;
}

void URsActiveEffectViewModel::Initialize()
{
	Super::Initialize();

	if (URsAbilitySystemComponent* ASC = CachedASC.Get())
	{
		if (const FActiveGameplayEffect* ActiveEffect = GetActiveEffect())
		{
			SetDuration(ActiveEffect->GetDuration());
		}
		
		ASC->OnGameplayEffectTimeChangeDelegate(CachedEffectHandle)->AddWeakLambda(this, [this](FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration)
		{
			HandleEffectTimeChange(EffectHandle, NewStartTime, NewDuration);
		});

		ASC->OnGameplayEffectRemoved_InfoDelegate(CachedEffectHandle)->AddWeakLambda(this, [this](const FGameplayEffectRemovalInfo& RemovalInfo)
		{
			HandleEffectRemoved(RemovalInfo);
		});
	}
}

void URsActiveEffectViewModel::Deinitialize()
{
	// unsafe to remove delegate bind, because ActiveEffect which has binded delegate might be already deleted in ASC
	Super::Deinitialize();
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
