// Copyright 2024 Team BH.


#include "RsPlayerController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RsCheatManager.h"
#include "GameFramework/HUD.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/UI/RsUILibrary.h"

ARsPlayerController::ARsPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PartyComponent = CreateDefaultSubobject<URsPartyComponent>(TEXT("PartyComponent"));
	LockOnComponent = CreateDefaultSubobject<URsLockOnComponent>(TEXT("LockOnComponent"));
	
	CheatClass = URsCheatManager::StaticClass();
}

void ARsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	LockOnComponent->SetController(this);
}

URsPartyComponent* ARsPlayerController::GetPartyComponent() const
{
	return PartyComponent;
}

void ARsPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AddLooseGameplayTag(RsGameplayTags::CHARACTER_PLAYERCONTROLLED);
	}
}

void ARsPlayerController::OnUnPossess()
{
	APawn* ReleasedPawn = GetPawn();
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->RemoveLooseGameplayTag(RsGameplayTags::CHARACTER_PLAYERCONTROLLED);
	}
	
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

bool ARsPlayerController::ShouldShowLoadingScreen(FString& OutReason) const
{
	if (ILoadingProcessInterface* LoadingInterface = Cast<ILoadingProcessInterface>(GetHUD()))
	{
		return LoadingInterface->ShouldShowLoadingScreen(OutReason);
	}
	return false;
}

URsLockOnComponent* ARsPlayerController::GetLockOnComponent() const
{
	return LockOnComponent;
}

void ARsPlayerController::LockControlRotation(bool bLock)
{
	bCanControlRotation = !bLock;
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
		for (const TTuple<FGameplayTag, UInputAction*>& OpenMenu : OpenMenuActions)
		{
			EnhancedInputComponent->BindAction(OpenMenu.Value, ETriggerEvent::Triggered, this, &ThisClass::HandleOpenMenu, OpenMenu.Key);
		}
		EnhancedInputComponent->BindAction(ShowCursorAction, ETriggerEvent::Triggered, this, &ThisClass::HandleShowCursor);
		EnhancedInputComponent->BindAction(ResetCursorAction, ETriggerEvent::Triggered, this, &ThisClass::HandleResetCursor);
	}
}

void ARsPlayerController::HandleShowCursor(const FInputActionValue& Value)
{
	URsUILibrary::ShowMouseCursor(this);
}

void ARsPlayerController::HandleResetCursor(const FInputActionValue& Value)
{
	URsUILibrary::ResetMouseCursor(this);
}

void ARsPlayerController::HandleOpenMenu(const FInputActionValue& Value, FGameplayTag WidgetTag)
{
	URsUILibrary::OpenMenuWidget(this, WidgetTag);
}
