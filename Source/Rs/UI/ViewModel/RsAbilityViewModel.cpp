// Copyright 2024 Team BH.


#include "RsAbilityViewModel.h"

#include "AbilitySystemComponent.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"

URsAbilityViewModel* URsAbilityViewModel::CreateRsAbilityViewModel(URsGameplayAbility* Model)
{
	URsAbilityViewModel* ViewModel = NewObject<URsAbilityViewModel>(Model);
	ViewModel->Initialize();
	return ViewModel;
}

void URsAbilityViewModel::Initialize()
{
	URsGameplayAbility* Model = Cast<URsGameplayAbility>(GetOuter());
	URsAbilitySystemComponent* RsASC = Cast<URsAbilitySystemComponent>(Model->GetAbilitySystemComponentFromActorInfo());
	if (RsASC == nullptr)
	{
		return;
	}
	
	// RsASC->OnGameplayEffectChanged.AddDynamic(this, &ThisClass::HandleGameplayEffectChanged);
	CooldownTag = Model->CooldownTag;
	
	if (Model->CooldownTag.IsValid())
	{
		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(Model->CooldownTag.GetSingleTagContainer());
		TArray<TPair<float, float>> DurationAndTimeRemaining = RsASC->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			SetCooldownRemaining(DurationAndTimeRemaining[BestIdx].Key);
			SetCooldownDuration(DurationAndTimeRemaining[BestIdx].Value);
		}
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

void URsAbilityViewModel::SetCooldownDuration(float NewCooldownDuration)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CooldownDuration, NewCooldownDuration))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownPercent);
	}
}

void URsAbilityViewModel::SetCooldownRemaining(float NewCooldownRemaining)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CooldownRemaining, NewCooldownRemaining))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownPercent);
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

void URsAbilityViewModel::HandleGameplayEffectChanged(FActiveGameplayEffect& Effect)
{
	if (Effect.Spec.DynamicGrantedTags.HasTag(CooldownTag))
	{
		SetCooldownDuration(Effect.GetDuration());
		SetCooldownRemaining(Effect.GetTimeRemaining(GetWorld()->TimeSeconds));
	}
}
