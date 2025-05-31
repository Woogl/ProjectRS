// Copyright 2024 Team BH.


#include "RsPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/UI/RsUILibrary.h"

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
	Super::OnPossess(InPawn);

	if (GameplayCameraComponent)
	{
		GameplayCameraComponent->ActivateCameraForPlayerController(this);
	}
}

void ARsPlayerController::OnUnPossess()
{
	APawn* ReleasedPawn = GetPawn();
	
	Super::OnUnPossess();

	// Reuse the cached Friendly AI Controller.
	if (ARsPlayerCharacter* RsCharacter = Cast<ARsPlayerCharacter>(ReleasedPawn))
	{
		if (TObjectPtr<AAIController> SpawnedAIController = RsCharacter->FriendlyAIController)
		{
			SpawnedAIController->Possess(ReleasedPawn);
		}
	}
}

UAbilitySystemComponent* ARsPlayerController::GetAbilitySystemComponent() const
{
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPawn());
}

UGameplayCameraComponent* ARsPlayerController::GetGameplayCameraComponent() const
{
	return GameplayCameraComponent;
}

URsLockOnComponent* ARsPlayerController::GetLockOnComponent() const
{
	return LockOnComponent;
}

APlayerController* ARsPlayerController::GetRsPlayerController(const UObject* WorldContextObject)
{
	return Cast<APlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

void ARsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(ControllerMappingContext, 0);
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ToggleCursorAction, ETriggerEvent::Triggered, this, &ThisClass::HandleToggleCursor);
	}
}

void ARsPlayerController::HandleToggleCursor(const FInputActionValue& Value)
{
	if (bShowMouseCursor)
	{
		URsUILibrary::HideCursor(this);
	}
	else
	{
		URsUILibrary::ShowCursor(this);
	}
}