// Copyright 2024 Team BH.


#include "RsPlayerState.h"

#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"

ARsPlayerState::ARsPlayerState()
{
	// If the NetUpdateFrequency is too low, there will be a delay on Ability activation / Effect application on the client.
	SetNetUpdateFrequency(100.f);

	// Create the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	// This will replicate minimal Gameplay Effects to Simulated Proxies and full info to everyone else.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* ARsPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
