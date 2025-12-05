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
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}
	
	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsStaggerSet::GetCurrentStaggerAttribute()).AddUObject(this, &ThisClass::HandleStaggerChange);
	StaggerSet = AbilitySystemComponent->GetSet<URsStaggerSet>();
	if (!StaggerSet)
	{
		UE_LOG(RsAbilityLog, Error, TEXT("Cannot initialize RsStaggerComponent for owner [%s] with NULL stagger set."), *GetNameSafe(GetOwner()));
		return;
	}
	
	//StaggerSet->OnStaggerChanged.AddUObject(this, &ThisClass::HandleStaggerChange);
	// StaggerSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChange);
	// StaggerSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);
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

void URsStaggerComponent::HandleStaggerChange(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnStaggerChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void URsStaggerComponent::OnRep_bIsGroggy(bool OldValue)
{
	if (OldValue == false && bIsGroggy == true && StaggerSet)
	{
		if (UAbilitySystemComponent* ASC = StaggerSet->GetOwningAbilitySystemComponent())
		{
			FGameplayEventData Payload;
			Payload.EventTag = RsGameplayTags::ABILITY_GROGGY;
			ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
		OnGroggyStarted.Broadcast(GetOwner());
	}
}