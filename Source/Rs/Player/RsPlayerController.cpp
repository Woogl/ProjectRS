// Copyright 2024 Team BH.


#include "RsPlayerController.h"

#include "Rs/Party/RsPartyComponent.h"

ARsPlayerController::ARsPlayerController()
{
	PartyComponent = CreateDefaultSubobject<URsPartyComponent>(TEXT("PartyComponent"));
}

void ARsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
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
