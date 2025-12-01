// Copyright 2024 Team BH.


#include "RsPlayerState.h"

#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Attributes/RsSpeedSet.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"


ARsPlayerState::ARsPlayerState()
{
	// Create the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	// This will replicate minimal Gameplay Effects to Simulated Proxies and full info to everyone else.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	HealthSet = CreateDefaultSubobject<URsHealthSet>(TEXT("HealthSet"));
	StaggerSet = CreateDefaultSubobject<URsStaggerSet>(TEXT("StaggerSet"));
	EnergySet = CreateDefaultSubobject<URsEnergySet>(TEXT("EnergySet"));
	AttackSet = CreateDefaultSubobject<URsAttackSet>(TEXT("AttackSet"));
	DefenseSet = CreateDefaultSubobject<URsDefenseSet>(TEXT("DefenseSet"));
	SpeedSet = CreateDefaultSubobject<URsSpeedSet>(TEXT("SpeedSet"));
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* ARsPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
