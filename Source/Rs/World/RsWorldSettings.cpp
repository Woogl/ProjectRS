// Copyright 2025 Team BH.


#include "RsWorldSettings.h"

#include "Rs/AbilitySystem/RsAbilitySet.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemSettings.h"

ARsWorldSettings::ARsWorldSettings()
{
	MinGlobalTimeDilation = 0.f;

	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(false);
}

void ARsWorldSettings::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<URsAbilitySet> AbilitySet = URsAbilitySystemSettings::Get().WorldAbilitySet.LoadSynchronous();
	if (AbilitySystemComponent && AbilitySet)
	{
		AbilitySystemComponent->InitializeAbilitySystem(AbilitySet, this, this);
	}
}

UAbilitySystemComponent* ARsWorldSettings::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
