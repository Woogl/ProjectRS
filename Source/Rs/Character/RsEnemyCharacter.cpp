// Copyright 2024 Team BH.


#include "RsEnemyCharacter.h"

#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/Component/RsHealthComponent.h"
#include "Rs/AbilitySystem/Component/RsStaggerComponent.h"

ARsEnemyCharacter::ARsEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set the pointer from Character Base to the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	// Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	TeamId = ERsTeamId::Enemy;
}

void ARsEnemyCharacter::BeginPlay()
{
	// The Ability System Component is created in the class constructor, so it should always be valid at this point.
	if (AbilitySystemComponent)
	{
		for (URsAbilitySet* AbilitySet : AbilitySets)
		{
			AbilitySystemComponent->InitializeAbilitySystem(AbilitySet, this, this);
		}
		HealthComponent->Initialize(AbilitySystemComponent);
		StaggerComponent->Initialize(AbilitySystemComponent);
		PostInitializeAbilitySystem();
	}
	
	Super::BeginPlay();
}
