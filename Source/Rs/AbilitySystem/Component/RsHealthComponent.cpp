// Copyright 2024 Team BH.


#include "RsHealthComponent.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/System/RsGameSetting.h"

URsHealthComponent::URsHealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void URsHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URsHealthComponent, bIsDead);
}

void URsHealthComponent::Initialize(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChange);
	HealthSet = AbilitySystemComponent->GetSet<URsHealthSet>();
	if (HealthSet)
	{
		OnHealthChange.Broadcast(HealthSet->GetCurrentHealth(), HealthSet->GetCurrentHealth(), nullptr);
	}
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

void URsHealthComponent::HandleHealthChange(const FOnAttributeChangeData& ChangeData)
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
	OnHealthChange.Broadcast(ChangeData.OldValue, ChangeData.NewValue, Instigator);

	if (ChangeData.NewValue <= 0.f && bIsDead == false)
	{
		bIsDead = true;
		OnRep_bIsDead(false);
		GetOwner()->ForceNetUpdate();
	}
}

void URsHealthComponent::OnRep_bIsDead(bool OldbIsDead)
{
	if (OldbIsDead == false && bIsDead == true)
	{
		if (UAbilitySystemComponent* ASC = HealthSet->GetOwningAbilitySystemComponent())
		{
			FGameplayEventData Payload;
			Payload.EventTag = URsGameSetting::Get()->DeathAbilityTag;
			ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
		OnDeathStarted.Broadcast(GetOwner());
	}
}
