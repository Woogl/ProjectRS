// Copyright 2024 Team BH.


#include "RsStaggerComponent.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"
#include "Rs/System/RsGameSettingDataAsset.h"

URsStaggerComponent::URsStaggerComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void URsStaggerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URsStaggerComponent, bIsGroggy);
}

void URsStaggerComponent::Initialize(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetCurrentStaggerAttribute()).AddUObject(this, &ThisClass::HandleStaggerChange);
	StaggerSet = AbilitySystemComponent->GetSet<URsStaggerSet>();
	if (StaggerSet)
	{
		OnStaggerChange.Broadcast(StaggerSet->GetCurrentStagger(), StaggerSet->GetCurrentStagger(), nullptr);
	}
}

float URsStaggerComponent::GetCurrentStagger()
{
	if (StaggerSet)
	{
		return StaggerSet->GetCurrentStagger();
	}
	return 0.f;
}

float URsStaggerComponent::GetMaxStagger()
{
	if (StaggerSet)
	{
		return StaggerSet->GetMaxStagger();
	}
	return 0.f;
}

void URsStaggerComponent::HandleStaggerChange(const FOnAttributeChangeData& ChangeData)
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
	OnStaggerChange.Broadcast(ChangeData.OldValue, ChangeData.NewValue, Instigator);

	bool GroggyCondition = ChangeData.NewValue >= GetMaxStagger();
	if (GroggyCondition != bIsGroggy)
	{
		bIsGroggy = !bIsGroggy;
		OnRep_bIsGroggy(!bIsGroggy);
		GetOwner()->ForceNetUpdate();
	}
}

void URsStaggerComponent::OnRep_bIsGroggy(bool OldValue)
{
	if (bIsGroggy == true)
	{
		if (UAbilitySystemComponent* ASC = StaggerSet->GetOwningAbilitySystemComponent())
		{
			FGameplayEventData Payload;
			Payload.EventTag = URsGameSettingDataAsset::Get().GroggyAbilityTag;
			ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
		OnGroggyEvent.Broadcast(GetOwner());
	}
}