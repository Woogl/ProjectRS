// Copyright 2024 Team BH.


#include "RsEnemyAIController.h"

ARsEnemyAIController::ARsEnemyAIController()
{
}

UAbilitySystemComponent* ARsEnemyAIController::GetAbilitySystemComponent() const
{
	return Super::GetAbilitySystemComponent();
}

void ARsEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ARsEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
