// Copyright 2024 Team BH.


#include "RsEnemyCharacter.h"

#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AI/AIController/RsEnemyAIController.h"
#include "Rs/UI/Component/RsNameplateComponent.h"


ARsEnemyCharacter::ARsEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	NameplateComponent = CreateDefaultSubobject<URsNameplateComponent>(TEXT("NameplateComponent"));
	NameplateComponent->SetupAttachment(RootComponent);
	NameplateComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));

	TeamId = ERsTeamId::Enemy;
	AIControllerClass = ARsEnemyAIController::StaticClass();
}

void ARsEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	check(NameplateComponent);
	NameplateComponent->Initialize(this);
}
