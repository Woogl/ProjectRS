// Copyright 2024 Team BH.


#include "RsActiveEffectViewModel.h"

#include "Components/SlateWrapperTypes.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/EffectComponent/RsUIDataEffectComponent.h"

URsActiveEffectViewModel::URsActiveEffectViewModel()
{
	SetTickableTickType(ETickableTickType::Conditional);
}

URsActiveEffectViewModel* URsActiveEffectViewModel::CreateRsActiveEffectViewModel(FActiveGameplayEffectHandle EffectHandle)
{
	UAbilitySystemComponent* ASC = EffectHandle.GetOwningAbilitySystemComponent();
	const FActiveGameplayEffect* ActiveEffect = ASC->GetActiveGameplayEffect(EffectHandle);
	if (!ASC || !ActiveEffect)
	{
		return nullptr;
	}
	
	// Active effect view model must have UI Data.
	const URsUIDataEffectComponent* UIData = FindRsUIData(*ActiveEffect);
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

ESlateVisibility URsActiveEffectViewModel::GetVisibility() const
{
	return GetActiveEffect() ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;
}

int32 URsActiveEffectViewModel::GetStack() const
{
	const FActiveGameplayEffect* ActiveEffect = GetActiveEffect();
	return ActiveEffect ? ActiveEffect->ClientCachedStackCount : 0;
}

FText URsActiveEffectViewModel::GetStackText() const
{
	int32 Stack = GetStack();
	return Stack > 1 ? FText::AsNumber(Stack) : FText();
}

float URsActiveEffectViewModel::GetEffectProgress() const
{
	const FActiveGameplayEffect* ActiveEffect = GetActiveEffect();
	return ActiveEffect ? ActiveEffect->GetTimeRemaining(GetWorld()->GetTimeSeconds()) / ActiveEffect->GetDuration() : 1.0f;
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

void URsActiveEffectViewModel::OnEffectAdded()
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIcon);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetVisibility);
}

void URsActiveEffectViewModel::OnEffectRenewed(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStack);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStackText);
}

void URsActiveEffectViewModel::OnEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	OnViewModelDisabled.Broadcast(this);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetVisibility);
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
		OnEffectAdded();
		ASC->OnGameplayEffectTimeChangeDelegate(CachedEffectHandle)->AddUObject(this, &ThisClass::OnEffectRenewed);
		ASC->OnGameplayEffectRemoved_InfoDelegate(CachedEffectHandle)->AddUObject(this, &ThisClass::OnEffectRemoved);
	}
}

void URsActiveEffectViewModel::Deinitialize()
{
	// unsafe to remove delegate bind, because ActiveEffect which has binded delegate might be already deleted in ASC
	Super::Deinitialize();
}

void URsActiveEffectViewModel::Tick(float DeltaTime)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
}
