// Copyright 2024 Team BH.


#include "RsActiveEffectViewModel.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/EffectComponent/RsGameplayEffectUIDataComponent.h"

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

float URsActiveEffectViewModel::GetEffectProgress() const
{
	if (CachedModel)
	{
		return CachedModel->GetTimeRemaining(GetWorld()->GetTimeSeconds()) / CachedModel->GetDuration();
	}
	return 0;
}

FText URsActiveEffectViewModel::GetStackDataText() const
{
	if (!CachedModel)
	{
		return FText();
	}
	FGameplayTagContainer EffectTags;
	CachedModel->Spec.GetAllAssetTags(EffectTags);
	if (int32 Stack = CachedASC->GetActiveEffectsWithAllTags(EffectTags).Num() > 1)
	{
		return FText::AsNumber(Stack);
	}
	return FText();
}

UTexture2D* URsActiveEffectViewModel::GetIcon() const
{
	if (const URsGameplayEffectUIDataComponent* RsUIData = FindRsUIData(*CachedModel))
	{
		return RsUIData->Icon;
	}
	return nullptr;
}

FText URsActiveEffectViewModel::GetDescription() const
{
	if (const URsGameplayEffectUIDataComponent* RsUIData = FindRsUIData(*CachedModel))
	{
		return RsUIData->Description;
	}
	return FText();
}

ERsActiveEffectCategory URsActiveEffectViewModel::GetCategory() const
{
	if (CachedModel)
	{
		FGameplayTagContainer EffectTags;
		CachedModel->Spec.GetAllAssetTags(EffectTags);
		if (EffectTags.HasTag(FGameplayTag::RequestGameplayTag("Dot")))
		{
			return ERsActiveEffectCategory::DotDamage;
		}
		if (EffectTags.HasTag(FGameplayTag::RequestGameplayTag("Debuff")))
		{
			return ERsActiveEffectCategory::Debuff;
		}
		if (EffectTags.HasTag(FGameplayTag::RequestGameplayTag("Buff")))
		{
			return ERsActiveEffectCategory::Buff;
		}
	}
	return ERsActiveEffectCategory::Others;
}

const URsGameplayEffectUIDataComponent* URsActiveEffectViewModel::FindRsUIData(const FActiveGameplayEffect& Effect)
{
	if (const UGameplayEffectComponent* UIData = Effect.Spec.Def->FindComponent(URsGameplayEffectUIDataComponent::StaticClass()))
	{
		return Cast<URsGameplayEffectUIDataComponent>(UIData);
	}
	return nullptr;
}

void URsActiveEffectViewModel::OnEffectAdded(const FActiveGameplayEffect& Effect)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStackDataText);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIcon);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCategory);
}

void URsActiveEffectViewModel::OnEffectRenewed(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStackDataText);
}

void URsActiveEffectViewModel::OnEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	BeginDestroy();
}

void URsActiveEffectViewModel::Initialize()
{
	Super::Initialize();

	CachedASC = Cast<URsAbilitySystemComponent>(GetOuter());
	if (CachedASC.Get() && CachedModel)
	{
		OnEffectAdded(*CachedModel);
		CachedASC.Get()->OnGameplayEffectTimeChangeDelegate(CachedModel->Handle)->AddUObject(this, &URsActiveEffectViewModel::OnEffectRenewed);
		CachedASC.Get()->OnGameplayEffectRemoved_InfoDelegate(CachedModel->Handle)->AddUObject(this, &URsActiveEffectViewModel::OnEffectRemoved);
	}
}

void URsActiveEffectViewModel::Deinitialize()
{
	if (CachedASC.Get() && CachedModel)
	{
		CachedASC.Get()->OnGameplayEffectTimeChangeDelegate(CachedModel->Handle)->AddUObject(this, &URsActiveEffectViewModel::OnEffectRenewed);
		CachedASC.Get()->OnGameplayEffectRemoved_InfoDelegate(CachedModel->Handle)->AddUObject(this, &URsActiveEffectViewModel::OnEffectRemoved);
	}
	Super::Deinitialize();
}

void URsActiveEffectViewModel::Tick(float DeltaTime)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
}