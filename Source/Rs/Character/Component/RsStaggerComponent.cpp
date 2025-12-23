// Copyright 2024 Team BH.


#include "RsStaggerComponent.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"

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
	OwnerAbilitySystemComponent = Cast<URsAbilitySystemComponent>(AbilitySystemComponent);
	if (OwnerAbilitySystemComponent == nullptr)
	{
		return;
	}
	
	StaggerSet = AbilitySystemComponent->GetSet<URsStaggerSet>();
	if (!StaggerSet)
	{
		UE_LOG(LogRsAbility, Error, TEXT("Cannot initialize RsStaggerComponent for owner [%s] with NULL stagger set."), *GetNameSafe(GetOwner()));
		return;
	}
	
	OwnerAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetCurrentStaggerAttribute()).AddUObject(this, &ThisClass::HandleStaggerChange);
}

void URsStaggerComponent::Deinitialize()
{
	if (OwnerAbilitySystemComponent)
	{
		OwnerAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetCurrentStaggerAttribute()).RemoveAll(this);
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

void URsStaggerComponent::HandleAbilitySystemInitialized()
{
	if (URsAbilitySystemComponent* RsASC = URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		Initialize(RsASC);
	}
}

void URsStaggerComponent::HandleStaggerChange(const FOnAttributeChangeData& Data)
{
	if (GetOwner()->HasAuthority())
    {
    	if (Data.NewValue >= GetMaxStagger() && !bIsGroggy)
		{
    		const bool bOldGroggy = bIsGroggy;
			bIsGroggy = true;
    		OnRep_bIsGroggy(bOldGroggy);
			GetOwner()->ForceNetUpdate();
		}
		else if (Data.NewValue < GetMaxStagger() && bIsGroggy)
		{
			const bool bOldGroggy = bIsGroggy;
			bIsGroggy = false;
			OnRep_bIsGroggy(bOldGroggy);
			GetOwner()->ForceNetUpdate();
		}
    }
    
    OnStaggerChanged.Broadcast(Data.OldValue, Data.NewValue);
}

void URsStaggerComponent::OnRep_bIsGroggy(bool OldValue)
{
	if (!OldValue && bIsGroggy)
	{
		if (OwnerAbilitySystemComponent)
		{
			FGameplayEventData Payload;
			Payload.EventTag = RsGameplayTags::ABILITY_GROGGY;
			OwnerAbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);

			OnGroggyStarted.Broadcast(GetOwner());
		}
	}
	else if (OldValue && !bIsGroggy)
	{
		// HandleGroggyEnded();
	}
}