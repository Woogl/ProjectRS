// Copyright 2024 Team BH.


#include "RsPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/Player/RsPlayerController.h"

ARsPlayerCharacter::ARsPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// This will replicate minimal Gameplay Effects to Simulated Proxies and full info to everyone else.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	GameplayCameraComponent = CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("GameplayCameraComponent"));
	GameplayCameraComponent->SetupAttachment(GetMesh());
	GameplayCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	GameplayCameraComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	GameplayCameraComponent->bAutoActivate = false;
	
	PerfectDodgeCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PerfectDodgeCapsuleComponent"));
	PerfectDodgeCapsuleComponent->SetupAttachment(RootComponent);
	PerfectDodgeCapsuleComponent->InitCapsuleSize(100.0f, 100.0f);
	PerfectDodgeCapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	PerfectDodgeCapsuleComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	PerfectDodgeCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	TeamId = ERsTeamId::Player;
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
	URsPartyLibrary::InsertPartyMemberAt(this, 0);
}

void ARsPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Server-side
	InitializeAbilitySystem();
	
	SetupCamera_Client();
}

void ARsPlayerCharacter::UnPossessed()
{
	// Unbind old ability inputs.
	check(AbilitySystemComponent);
	AbilitySystemComponent->TearDownAbilityInputBindings();
	
	Super::UnPossessed();

	GetMovementComponent()->StopMovementImmediately();
	
	check(GameplayCameraComponent);
	GameplayCameraComponent->DeactivateCamera();
}

void ARsPlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	
	// Client-side
	InitializeAbilitySystem();
}

void ARsPlayerCharacter::InitializeAbilitySystem()
{
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// Bind ability input for player controlled character.
	if (IsPlayerControlled() && IsLocallyControlled())
	{
		AbilitySystemComponent->SetupAbilityInputBindings();
	}
}

void ARsPlayerCharacter::SetupCamera_Client_Implementation()
{
	if (Controller)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		GameplayCameraComponent->ActivateCameraForPlayerController(PlayerController);
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

ERsCameraRig ARsPlayerCharacter::GetCameraRig() const
{
	return CameraRig;
}

void ARsPlayerCharacter::SetCameraRig(ERsCameraRig InCameraRig)
{
	CameraRig = InCameraRig;
}

UGameplayCameraComponent* ARsPlayerCharacter::GetGameplayCameraComponent() const
{
	return GameplayCameraComponent;
}

UCapsuleComponent* ARsPlayerCharacter::GetPerfectDodgeCapsuleComponent() const
{
	return PerfectDodgeCapsuleComponent;
}

UInputMappingContext* ARsPlayerCharacter::GetDefaultMappingContext() const
{
	return DefaultMappingContext;
}
