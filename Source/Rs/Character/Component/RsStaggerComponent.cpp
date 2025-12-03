// Copyright 2024 Team BH.


#include "RsStaggerComponent.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"
#include "Rs/System/RsGameSettingDataAsset.h"

URsStaggerComponent::URsStaggerComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void URsStaggerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (URsAbilitySystemComponent* RsASC = URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		RsASC->CallOrRegister_OnAbilitySystemInitialized(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::HandleAbilitySystemInitialized));
	}
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

void URsStaggerComponent::HandleAbilitySystemInitialized()
{
	if (URsAbilitySystemComponent* RsASC = URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		Initialize(RsASC);
	}
}

void URsStaggerComponent::HandleStaggerChange(const FOnAttributeChangeData& ChangeData)
{
	OnStaggerChange.Broadcast(ChangeData.OldValue, ChangeData.NewValue);

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
	if (OldValue == false && bIsGroggy == true)
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