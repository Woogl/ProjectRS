// Copyright 2024 Team BH.


#include "RsPlayerController.h"

#include "GameFramework/GameplayCameraComponent.h"
#include "GameFramework/GameplayControlRotationComponent.h"
#include "Rs/Party/RsPartyComponent.h"

ARsPlayerController::ARsPlayerController()
{
	PartyComponent = CreateDefaultSubobject<URsPartyComponent>(TEXT("PartyComponent"));

	GameplayCameraComponent = CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("GameplayCameraComponent"));
	GameplayCameraComponent->SetupAttachment(GetRootComponent());

	GameplayControlRotationComponent = CreateDefaultSubobject<UGameplayControlRotationComponent>(TEXT("GameplayControlRotationComponent"));
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

	if (GameplayCameraComponent)
	{
		GameplayCameraComponent->ActivateCameraForPlayerIndex(0);
	}
}

TObjectPtr<AController> ARsPlayerController::GetPrevController() const
{
	return PrevController;
}

void ARsPlayerController::SetPrevController(AController* Controller)
{
	PrevController = Controller;
}
