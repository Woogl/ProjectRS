// Copyright 2024 Team BH.


#include "RsEnemyCharacter.h"

#include "Component/RsHealthComponent.h"
#include "Component/RsStaggerComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
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
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);

	TeamId = ERsTeamId::Enemy;
}

void ARsEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeAbilitySystem();
}

void ARsEnemyCharacter::InitializeAbilitySystem()
{
	Super::InitializeAbilitySystem();
	
	// The Ability System Component is created in the class constructor, so it should always be valid at this point.
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		for (URsAbilitySet* AbilitySet : AbilitySets)
		{
			AbilitySystemComponent->InitializeAbilitySet(AbilitySet);
		}
		HealthComponent->Initialize(AbilitySystemComponent);
		StaggerComponent->Initialize(AbilitySystemComponent);
		NameplateComponent->Initialize(this);
	}
}
