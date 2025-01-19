// Copyright 2024 Team BH.


#include "RsPlayerController.h"

#include "Rs/Party/RsPartyComponent.h"

ARsPlayerController::ARsPlayerController()
{
	PartyComponent = CreateDefaultSubobject<URsPartyComponent>(TEXT("PartyComponent"));
}

URsPartyComponent* ARsPlayerController::GetPartyComponent() const
{
	return PartyComponent;
}

void ARsPlayerController::OnPossess(APawn* InPawn)
{
	PrevController = InPawn->GetController();
	Super::OnPossess(InPawn);
}

TObjectPtr<AController> ARsPlayerController::GetPrevController() const
{
	return PrevController;
}
