// Copyright 2024 Team BH.


#include "RsActiveEffectViewModel.h"

#include "Components/SlateWrapperTypes.h"
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
		ViewModel->CachedModel = EffectHandle;
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
	if (CachedASC.Get() && CachedModel.IsValid())
	{
		if (GetCategory() != ERsEffectCategory::DotDamage)
		{
			if (const FActiveGameplayEffect* ActiveEffect = CachedASC->GetActiveGameplayEffect(CachedModel))
			{
				return ActiveEffect->GetTimeRemaining(GetWorld()->GetTimeSeconds()) / ActiveEffect->GetDuration();
			}
		}
	}
	return 1;
}


UTexture2D* URsActiveEffectViewModel::GetIcon() const
{
	if (CachedASC.Get() && CachedModel.IsValid())
	{
		if (const FActiveGameplayEffect* ActiveEffect = CachedASC->GetActiveGameplayEffect(CachedModel))
		{
			return FindRsUIData(*ActiveEffect)->Icon;
		}
	}
	return nullptr;
}

FText URsActiveEffectViewModel::GetDescription() const
{
	if (CachedASC.Get() && CachedModel.IsValid())
	{
		if (const FActiveGameplayEffect* ActiveEffect = CachedASC->GetActiveGameplayEffect(CachedModel))
		{
			return FindRsUIData(*ActiveEffect)->Description;
		}
	}
	return FText();
}

ERsEffectCategory URsActiveEffectViewModel::GetCategory() const
{
	if (CachedASC.Get() && CachedModel.IsValid())
	{
		if (const FActiveGameplayEffect* ActiveEffect = CachedASC->GetActiveGameplayEffect(CachedModel))
		{
			return FindRsUIData(*ActiveEffect)->Category;
		}
	}
	return ERsEffectCategory::Invalid;
}

void URsActiveEffectViewModel::AddExtraModel(FActiveGameplayEffectHandle OtherEffectHandle)
{
	if (CachedASC.Get() && CachedModel.IsValid())
	{
		if (const FActiveGameplayEffect* ActiveEffect = CachedASC->GetActiveGameplayEffect(CachedModel))
		{
			const FActiveGameplayEffect* OtherModelEffect = CachedASC->GetActiveGameplayEffect(OtherEffectHandle);
			if (FindRsUIData(*ActiveEffect) == FindRsUIData(*OtherModelEffect))
			{
				ExtraModels.Add(OtherEffectHandle);
				CachedASC.Get()->OnGameplayEffectTimeChangeDelegate(OtherEffectHandle)->AddUObject(this, &ThisClass::OnEffectRenewed);
				CachedASC.Get()->OnGameplayEffectRemoved_InfoDelegate(OtherEffectHandle)->AddUObject(this, &ThisClass::OnEffectRemoved);
				Stack++;
				UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStack);
				UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetStackText);
			}
		}
	}
}

const URsGameplayEffectUIDataComponent* URsActiveEffectViewModel::FindRsUIData(const FActiveGameplayEffect& Effect)
{
	if (const UGameplayEffectComponent* UIData = Effect.Spec.Def->FindComponent(URsGameplayEffectUIDataComponent::StaticClass()))
	{
		return Cast<URsGameplayEffectUIDataComponent>(UIData);
	}
	return nullptr;
}

void URsActiveEffectViewModel::OnEffectAdded(FActiveGameplayEffectHandle EffectHandle)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIcon);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCategory);
}

void URsActiveEffectViewModel::OnEffectRenewed(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration)
{
	// not nessesary
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
}

void URsActiveEffectViewModel::OnEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	FActiveGameplayEffectHandle RemovedModel = RemovalInfo.ActiveEffect->Handle;
	int32 Index = ExtraModels.Find(RemovedModel);

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
	if (CachedASC.Get() && CachedModel.IsValid())
	{
		OnEffectAdded(CachedModel);
		CachedASC.Get()->OnGameplayEffectTimeChangeDelegate(CachedModel)->AddUObject(this, &ThisClass::OnEffectRenewed);
		CachedASC.Get()->OnGameplayEffectRemoved_InfoDelegate(CachedModel)->AddUObject(this, &ThisClass::OnEffectRemoved);
		UE_MVVM_SET_PROPERTY_VALUE(Visibility, ESlateVisibility::HitTestInvisible);
	}
}

void URsActiveEffectViewModel::Deinitialize()
{
	Super::Deinitialize();
}

bool URsActiveEffectViewModel::IsTickable() const
{
	ERsEffectCategory Category = GetCategory();
	return (Category != ERsEffectCategory::DotDamage) && (Category != ERsEffectCategory::Invalid);
}

void URsActiveEffectViewModel::Tick(float DeltaTime)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEffectProgress);
}
