// Copyright 2024 Team BH.


#include "RsPlayerState.h"

#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"

ARsPlayerState::ARsPlayerState()
{
	// Create the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	// This will replicate minimal Gameplay Effects to Simulated Proxies and full info to everyone else.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AbilitySystemComponent->SetIsReplicated(true);
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* ARsPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
