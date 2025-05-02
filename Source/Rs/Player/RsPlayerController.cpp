// Copyright 2024 Team BH.


#include "RsPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Rs/RsLogChannels.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Party/RsPartyComponent.h"

ARsPlayerController::ARsPlayerController()
{
	PartyComponent = CreateDefaultSubobject<URsPartyComponent>(TEXT("PartyComponent"));

	LockOnComponent = CreateDefaultSubobject<URsLockOnComponent>(TEXT("LockOnComponent"));

	GameplayCameraComponent = CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("GameplayCameraComponent"));
	GameplayCameraComponent->SetupAttachment(GetRootComponent());
	GameplayCameraComponent->AutoActivateForPlayer = EAutoReceiveInput::Player0;
	GameplayCameraComponent->bAutoActivate = true;
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
				if (GetCharacter()->AIControllerClass->IsChildOf(AAIController::StaticClass()))
				{
					PrevAIController = GetWorld()->SpawnActor<AAIController>(GetCharacter()->AIControllerClass);
				}
				else
				{
					UE_LOG(RsLog, Error, TEXT("%s has Invalid AIController. Please use 'BP_FriendlyAIController' instead."), *GetCharacter()->GetName());
				}
			}
		}
	}
	
	Super::OnPossess(InPawn);

	if (GameplayCameraComponent)
	{
		GameplayCameraComponent->ActivateCameraForPlayerController(this);
	}
}

UAbilitySystemComponent* ARsPlayerController::GetAbilitySystemComponent() const
{
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPawn());
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
