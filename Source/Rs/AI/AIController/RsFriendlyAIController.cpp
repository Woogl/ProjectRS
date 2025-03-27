// Copyright 2024 Team BH.


#include "RsFriendlyAIController.h"

ARsFriendlyAIController::ARsFriendlyAIController()
{
	// RsPlayerCharacter want player state to initialize ability system.
	bWantsPlayerState = true;
}

UAbilitySystemComponent* ARsFriendlyAIController::GetAbilitySystemComponent() const
{
	return Super::GetAbilitySystemComponent();
}

void ARsFriendlyAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARsFriendlyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}

