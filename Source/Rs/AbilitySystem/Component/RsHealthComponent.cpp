// Copyright 2024 Team BH.


#include "RsHealthComponent.h"

#include "GameplayEffectExtension.h"
#include "Rs/AbilitySystem/Attributes/HealthSet.h"

URsHealthComponent::URsHealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void URsHealthComponent::Initialize(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthSet::GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);
	HealthSet = AbilitySystemComponent->GetSet<UHealthSet>();

	OnHealthChanged.Broadcast(HealthSet->GetCurrentHealth(), HealthSet->GetCurrentHealth(), nullptr);
}

float URsHealthComponent::GetCurrentHealth()
{
	if (HealthSet)
	{
		return HealthSet->GetCurrentHealth();
	}
	return 0.f;
}

float URsHealthComponent::GetMaxHealth()
{
	if (HealthSet)
	{
		return HealthSet->GetMaxHealth();
	}
	return 0.f;
}

void URsHealthComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	AActor* Instigator = nullptr;
	if (const FGameplayEffectModCallbackData* EffectModData = ChangeData.GEModData)
	{
		const FGameplayEffectContextHandle& EffectContext = EffectModData->EffectSpec.GetEffectContext();
		if (EffectContext.IsValid())
		{
			Instigator = EffectContext.GetOriginalInstigator();
		}
	}
	
	OnHealthChanged.Broadcast(ChangeData.OldValue, ChangeData.NewValue, Instigator);
}
