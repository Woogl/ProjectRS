// Copyright 2024 Team BH.


#include "RsPlayerCharacter.h"

#include "AbilitySystemGlobals.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/Component/RsHealthComponent.h"
#include "Rs/Party/RsPartyLibrary.h"

ARsPlayerCharacter::ARsPlayerCharacter()
{
	HealthComponent = CreateDefaultSubobject<URsHealthComponent>(TEXT("HealthComponent"));

	// Team ID "0" is for player.
	TeamID = 0;
}

void ARsPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::HandleLook);
	}
}

void ARsPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Register Party member
	URsPartyLibrary::InsertPartyMember(this, 0);
}

void ARsPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server side
	InitAbilitySystem();
}

void ARsPlayerCharacter::UnPossessed()
{
	Super::UnPossessed();

	GetMovementComponent()->StopMovementImmediately();
}

void ARsPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Client side
	InitAbilitySystem();
}

void ARsPlayerCharacter::InitAbilitySystem()
{
	// Initialize the ASC once.
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<URsAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPlayerState()));
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->InitializeAbilitySystem(AbilitySet, GetPlayerState(), this);
			HealthComponent->Initialize(AbilitySystemComponent);
			PostInitializeAbilitySystem();
		}
	}

	// Unbind ability input for AI controlled character.
	// Bind ability input for player controlled character.
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityInputBindings();
	}
}

void ARsPlayerCharacter::HandleMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARsPlayerCharacter::HandleLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
