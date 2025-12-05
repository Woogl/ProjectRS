// Copyright 2024 Team BH.


#include "RsEnemyCharacter.h"

#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"


ARsEnemyCharacter::ARsEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	TeamId = ERsTeamId::Enemy;
}

void ARsEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
