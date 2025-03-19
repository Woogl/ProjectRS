// Copyright 2024 Team BH.


#include "RsPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "GameFramework/GameplayControlRotationComponent.h"
#include "GameFramework/PlayerState.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Party/RsPartyComponent.h"

ARsPlayerController::ARsPlayerController()
{
	PartyComponent = CreateDefaultSubobject<URsPartyComponent>(TEXT("PartyComponent"));

	LockOnComponent = CreateDefaultSubobject<URsLockOnComponent>(TEXT("LockOnComponent"));

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
	if (GetCharacter())
	{
		if (GetCharacter()->IsBotControlled())
		{
			// When AI controlled, keep old controller.
			PrevAIController = Cast<AAIController>(GetCharacter()->GetController());
		}
		else
		{
			// When player controlled, spawn an AI controller and keep it.
			if (PrevAIController == nullptr && GetCharacter()->AIControllerClass)
			{
				PrevAIController = GetWorld()->SpawnActor<AAIController>(GetCharacter()->AIControllerClass);
			}
		}
	}
	
	Super::OnPossess(InPawn);

	if (GameplayCameraComponent)
	{
		GameplayCameraComponent->ActivateCameraForPlayerIndex(0);
	}
}

UAbilitySystemComponent* ARsPlayerController::GetAbilitySystemComponent() const
{
	if (APlayerState* MyPlayerState = GetPlayerState<APlayerState>())
	{
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MyPlayerState);
	}
	return nullptr;
}

AAIController* ARsPlayerController::GetPrevAIController() const
{
	return PrevAIController;
}

UGameplayCameraComponent* ARsPlayerController::GetGameplayCameraComponent() const
{
	return GameplayCameraComponent;
}

URsLockOnComponent* ARsPlayerController::GetLockOnComponent() const
{
	return LockOnComponent;
}
