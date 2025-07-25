// Copyright 2024 Team BH.


#include "RsActiveEffectViewModel.h"

#include "Components/SlateWrapperTypes.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/EffectComponent/RsGameplayEffectUIDataComponent.h"

URsActiveEffectViewModel::URsActiveEffectViewModel()
{
	SetTickableTickType(ETickableTickType::Conditional);
}

URsActiveEffectViewModel* URsActiveEffectViewModel::CreateRsActiveEffectViewModel(FActiveGameplayEffectHandle EffectHandle)
{
	UAbilitySystemComponent* ASC = EffectHandle.GetOwningAbilitySystemComponent();
	const FActiveGameplayEffect* Effect = ASC->GetActiveGameplayEffect(EffectHandle);
	if (ASC && Effect && FindRsUIData(*Effect))
	{
		URsActiveEffectViewModel* ViewModel = NewObject<URsActiveEffectViewModel>(ASC);
		ViewModel->CachedModel = Effect;
		ViewModel->Initialize();
		return ViewModel;
	}
	return nullptr;
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
	if (CachedASC.Get() && CachedModel)
	{
		return CachedModel->GetTimeRemaining(GetWorld()->GetTimeSeconds()) / CachedModel->GetDuration();
	}
	return 1;
}


UObject* URsActiveEffectViewModel::GetIcon() const
{
	if (CachedASC.Get() && CachedModel)
	{
		if (const URsGameplayEffectUIDataComponent* UIData = FindRsUIData(*CachedModel))
		{
			return UIData->Icon;
		}
	}
	return nullptr;
}

FText URsActiveEffectViewModel::GetDescription() const
{
	if (CachedASC.Get() && CachedModel)
	{
		if (const URsGameplayEffectUIDataComponent* UIData = FindRsUIData(*CachedModel))
		{
			return UIData->Description;
		}
	}
	return FText();
}

void URsActiveEffectViewModel::AddExtraModel(FActiveGameplayEffectHandle OtherEffectHandle)
{
	if (CachedASC.Get() && CachedModel)
	{
		const FActiveGameplayEffect* OtherModelEffect = CachedASC->GetActiveGameplayEffect(OtherEffectHandle);
		if (FindRsUIData(*CachedModel) == FindRsUIData(*OtherModelEffect))
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

const URsGameplayEffectUIDataComponent* URsActiveEffectViewModel::FindRsUIData(const FActiveGameplayEffect& Effect)
{
	if (TObjectPtr<const UGameplayEffect> EffectDef = Effect.Spec.Def)
	{
		if (const UGameplayEffectComponent* UIData = EffectDef->FindComponent(URsGameplayEffectUIDataComponent::StaticClass()))
		{
			return Cast<URsGameplayEffectUIDataComponent>(UIData);
		}
	}
	return nullptr;
}

void URsActiveEffectViewModel::OnEffectAdded()
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIcon);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
	if (CachedASC.Get() && CachedModel)
	{
		FGameplayTagContainer ModelTags;
		CachedModel->Spec.GetAllAssetTags(ModelTags);
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
		CachedModel = ExtraModels[0];
		ExtraModels.RemoveAt(0);
	}
	else
	{
		UE_MVVM_SET_PROPERTY_VALUE(Visibility, ESlateVisibility::Collapsed);
		OnViewModelDisabled.Broadcast(this);
		return;
	}

	Stack--;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStack);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStackText);
}

void URsActiveEffectViewModel::Initialize()
{
	Super::Initialize();

	CachedASC = Cast<URsAbilitySystemComponent>(GetOuter());
	if (CachedASC.Get() && CachedModel)
	{
		OnEffectAdded();
		CachedASC.Get()->OnGameplayEffectTimeChangeDelegate(CachedModel->Handle)->AddUObject(this, &ThisClass::OnEffectRenewed);
		CachedASC.Get()->OnGameplayEffectRemoved_InfoDelegate(CachedModel->Handle)->AddUObject(this, &ThisClass::OnEffectRemoved);
		UE_MVVM_SET_PROPERTY_VALUE(Visibility, ESlateVisibility::HitTestInvisible);
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
