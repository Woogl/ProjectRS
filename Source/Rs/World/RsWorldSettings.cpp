// Copyright 2025 Team BH.


#include "RsWorldSettings.h"

#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"

ARsWorldSettings::ARsWorldSettings()
{
	MinGlobalTimeDilation = 0.f;

	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(false);
}

UAbilitySystemComponent* ARsWorldSettings::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
