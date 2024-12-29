// Copyright 2024 Team BH.


#include "RsEnemyCharacter.h"

#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"

ARsEnemyCharacter::ARsEnemyCharacter()
{
	// Set the pointer from Character Base to the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void ARsEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitializeAbilitySystem(AbilitySet, this, this);
		PostInitializeAbilitySystem();
	}
}
