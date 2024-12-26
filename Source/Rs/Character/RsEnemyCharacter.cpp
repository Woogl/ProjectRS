// Copyright 2024 Team BH.


#include "RsEnemyCharacter.h"

#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"

ARsEnemyCharacter::ARsEnemyCharacter()
{
	// Set the pointer from Character Base to the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>("Ability System Component");
}

void ARsEnemyCharacter::BeginPlay()
{
	// if (URsAbilitySystemComponent* RsAbilitySystemComponent = Cast<URsAbilitySystemComponent>(AbilitySystemComponent))
	// {
	// 	// Call the function on "Custom Ability System Component" to set up references and Init data. (Client and Server)
	// 	RsAbilitySystemComponent->InitializeAbilitySystemData(AbilitySystemInitializationData, this, this);
	// 	
	// }

	Super::BeginPlay();

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
}
