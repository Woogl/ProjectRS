// Copyright 2024 Team BH.


#include "RsPlayerState.h"

#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"

ARsPlayerState::ARsPlayerState()
{
	// If the NetUpdateFrequency is too low, there will be a delay on Ability activation / Effect application on the client.
	SetNetUpdateFrequency(100.f);

	// Create the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>("Ability System Component");
}

UAbilitySystemComponent* ARsPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
