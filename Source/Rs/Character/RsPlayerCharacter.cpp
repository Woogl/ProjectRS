// Copyright 2024 Team BH.


#include "RsPlayerCharacter.h"

#include "AbilitySystemGlobals.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"

ARsPlayerCharacter::ARsPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ARsPlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ARsPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Set up move action binding
		for (const FEnhancedActionKeyMapping& Mapping : DefaultMappingContext->GetMappings())
		{
			if (Mapping.Action && Mapping.Action->GetFName() == FName("IA_Move"))
			{
				EnhancedInputComponent->BindAction(Mapping.Action, ETriggerEvent::Triggered, this, &ThisClass::HandleMove);
				break;
			}
		}

		// Set up look action binding
		for (const FEnhancedActionKeyMapping& Mapping : DefaultMappingContext->GetMappings())
		{
			if (Mapping.Action && Mapping.Action->GetFName() == FName("IA_Look"))
			{
				EnhancedInputComponent->BindAction(Mapping.Action, ETriggerEvent::Triggered, this, &ThisClass::HandleLook);
				break;
			}
		}
	}
}

void ARsPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPlayerState());
	if (URsAbilitySystemComponent* RsAbilitySystemComponent = Cast<URsAbilitySystemComponent>(AbilitySystemComponent))
	{
		RsAbilitySystemComponent->InitAbilityActorInfo(GetPlayerState(), this);
	}
}

void ARsPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPlayerState());
	if (URsAbilitySystemComponent* RsAbilitySystemComponent = Cast<URsAbilitySystemComponent>(AbilitySystemComponent))
	{
		RsAbilitySystemComponent->InitAbilityActorInfo(GetPlayerState(), this);
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
