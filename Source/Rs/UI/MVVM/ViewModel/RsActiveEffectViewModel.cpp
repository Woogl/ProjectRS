// Copyright 2024 Team BH.


#include "RsActiveEffectViewModel.h"

#include "Components/SlateWrapperTypes.h"
#include "Rs/RsLogChannels.h"
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
	if (GetActiveEffect())
	{
		return ESlateVisibility::HitTestInvisible;
	}
	else
	{
		return ESlateVisibility::Collapsed;
	}
}

int32 URsActiveEffectViewModel::GetStack() const
{
	return Stack;
}

FText URsActiveEffectViewModel::GetStackText() const
{
	if (Stack > 1)
	{
		return FText::AsNumber(Stack);
	}
	return FText();
}

float URsActiveEffectViewModel::GetEffectProgress() const
{
	if (const FActiveGameplayEffect* ActiveEffect = GetActiveEffect())
	{
		return ActiveEffect->GetTimeRemaining(GetWorld()->GetTimeSeconds()) / ActiveEffect->GetDuration();
	}
	return 1.f;
}

UObject* URsActiveEffectViewModel::GetIcon() const
{
	if (const URsUIDataEffectComponent* UIData = CachedUIData.Get())
	{
		return UIData->Icon;
	}
	return nullptr;
}

FText URsActiveEffectViewModel::GetDescription() const
{
	if (const URsUIDataEffectComponent* UIData = CachedUIData.Get())
	{
		return UIData->Description;
	}
	return FText();
}

void URsActiveEffectViewModel::AddExtraModel(FActiveGameplayEffectHandle OtherEffectHandle)
{
	if (GetActiveEffect() && CachedUIData.IsValid())
	{
		const FActiveGameplayEffect* OtherModelEffect = CachedASC->GetActiveGameplayEffect(OtherEffectHandle);
		if (CachedUIData == FindRsUIData(*OtherModelEffect))
		{
			ExtraModels.Add(OtherModelEffect);
			CachedASC.Get()->OnGameplayEffectTimeChangeDelegate(OtherEffectHandle)->AddUObject(this, &ThisClass::OnEffectRenewed);
			CachedASC.Get()->OnGameplayEffectRemoved_InfoDelegate(OtherEffectHandle)->AddUObject(this, &ThisClass::OnEffectRemoved);
			Stack++;
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStack);
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStackText);
		}
	}
}

const URsUIDataEffectComponent* URsActiveEffectViewModel::FindRsUIData(const FActiveGameplayEffect& Effect)
{
	if (TObjectPtr<const UGameplayEffect> EffectDef = Effect.Spec.Def)
	{
		if (!Effect.IsPendingRemove)
		{
			if (const URsUIDataEffectComponent* UIData = EffectDef->FindComponent<URsUIDataEffectComponent>())
			{
				return UIData;
			}
		}
	}
	return nullptr;
}

void URsActiveEffectViewModel::OnEffectAdded()
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIcon);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetVisibility);
	if (GetActiveEffect())
	{
		FGameplayTagContainer ModelTags;
		GetActiveEffect()->Spec.GetAllAssetTags(ModelTags);
		if (ModelTags.HasTag(FGameplayTag::RequestGameplayTag("Effect.Damage.Dot")))
		{
			IsStackable = true;
		}
	}
}

void URsActiveEffectViewModel::OnEffectRenewed(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration)
{
	// not nessesary
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
}

void URsActiveEffectViewModel::OnEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	int32 Index = ExtraModels.Find(RemovalInfo.ActiveEffect);
	
	if (Index != INDEX_NONE)
	{
		ExtraModels.RemoveAt(Index);
	}
	else if (ExtraModels.Num() > 0)
	{
		// Change datas
		CachedEffectHandle = ExtraModels[0]->Handle;
		CachedUIData = FindRsUIData(*GetActiveEffect());
		ExtraModels.RemoveAt(0);
	}
	else
	{
		OnViewModelDisabled.Broadcast(this);
		return;
	}

	Stack--;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetVisibility);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStack);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStackText);
}

const FActiveGameplayEffect* URsActiveEffectViewModel::GetActiveEffect() const
{
	if (URsAbilitySystemComponent* ASC = CachedASC.Get())
	{
		return ASC->GetActiveGameplayEffect(CachedEffectHandle);
	}
	return nullptr;
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
	Super::Deinitialize();
}

bool URsActiveEffectViewModel::IsTickable() const
{
	return !IsStackable;
}

void URsActiveEffectViewModel::Tick(float DeltaTime)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
}
