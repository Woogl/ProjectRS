// Copyright 2024 Team BH.


#include "RsPlayerController.h"

#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/Party/RsPartySubsystem.h"

ARsPlayerController::ARsPlayerController()
{
	PartyComponent = CreateDefaultSubobject<URsPartyComponent>(TEXT("PartyComponent"));
}

void ARsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	
	if (URsPartySubsystem* PartySubsystem = URsPartySubsystem::Get(GetLocalPlayer()))
	{
		TArray<ARsPlayerCharacter*> SpawnedMembers = PartySubsystem->SpawnPartyMembers(GetPawn()->GetActorTransform());
		if (!SpawnedMembers.IsEmpty())
		{
			Possess(SpawnedMembers[0]);
		}
	}
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
