// Copyright 2024 Team BH.


#include "RsEnemyCharacter.h"

#include "Component/RsHealthComponent.h"
#include "Component/RsStaggerComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Attributes/RsSpeedSet.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"
#include "Rs/UI/Component/RsNameplateComponent.h"


ARsEnemyCharacter::ARsEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	
	// Set the pointer from Character Base to the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	// Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	HealthSet = CreateDefaultSubobject<URsHealthSet>(TEXT("HealthSet"));
	StaggerSet = CreateDefaultSubobject<URsStaggerSet>(TEXT("StaggerSet"));
	EnergySet = CreateDefaultSubobject<URsEnergySet>(TEXT("EnergySet"));
	AttackSet = CreateDefaultSubobject<URsAttackSet>(TEXT("AttackSet"));
	DefenseSet = CreateDefaultSubobject<URsDefenseSet>(TEXT("DefenseSet"));
	SpeedSet = CreateDefaultSubobject<URsSpeedSet>(TEXT("SpeedSet"));
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);

	TeamId = ERsTeamId::Enemy;
}

void ARsEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		InitAbilitySystem();
	}
}

void ARsEnemyCharacter::InitAbilitySystem()
{
	Super::InitAbilitySystem();
	
	// The Ability System Component is created in the class constructor, so it should always be valid at this point.
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitializeAbilitySystem(AbilitySets, this, this);
		
		HealthComponent->Initialize(AbilitySystemComponent);
		StaggerComponent->Initialize(AbilitySystemComponent);
		NameplateComponent->Initialize(this);
	}
}
