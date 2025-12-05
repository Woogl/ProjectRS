// Copyright 2024 Team BH.


#include "RsFriendlyAIController.h"

#include "Rs/Character/RsPlayerCharacter.h"

ARsFriendlyAIController::ARsFriendlyAIController()
{
}

void ARsFriendlyAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARsFriendlyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ARsPlayerCharacter* RsCharacter = Cast<ARsPlayerCharacter>(InPawn))
	{
		RsCharacter->FriendlyAIController = this;
	}
}

