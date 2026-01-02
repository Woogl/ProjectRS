// Copyright 2024 Team BH.


#include "RsPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AI/AIController/RsFriendlyAIController.h"
#include "Rs/Party/RsPartyLibrary.h"

ARsPlayerCharacter::ARsPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// This will replicate minimal Gameplay Effects to Simulated Proxies and full info to everyone else.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	JustDodgeCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("JustDodgeCapsule"));
	JustDodgeCapsule->SetupAttachment(RootComponent);
	JustDodgeCapsule->SetCapsuleHalfHeight(180.f);
	JustDodgeCapsule->SetCapsuleRadius(180.f);
	JustDodgeCapsule->SetCollisionProfileName(TEXT("Pawn"));
	JustDodgeCapsule->SetCollisionResponseToAllChannels(ECR_Overlap);
	EnableJustDodgeCapsule(false);
	
	GameplayCameraComponent = CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("GameplayCameraComponent"));
	GameplayCameraComponent->SetupAttachment(GetMesh());
	GameplayCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	GameplayCameraComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	GameplayCameraComponent->bAutoActivate = false;
	
	TeamId = ERsTeamId::Player;
	AIControllerClass = ARsFriendlyAIController::StaticClass();
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
	SetupAbilityInput();
	
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

void ARsPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->RefreshAbilityActorInfo();
}

void ARsPlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->RefreshAbilityActorInfo();
	
	// Client-side
	SetupAbilityInput();
}

void ARsPlayerCharacter::SetupAbilityInput()
{
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

void ARsPlayerCharacter::EnableJustDodgeCapsule(bool bEnable)
{
	check(JustDodgeCapsule);
	ECollisionEnabled::Type CollisionType = bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;
	JustDodgeCapsule->SetCollisionEnabled(CollisionType);
}

UGameplayCameraComponent* ARsPlayerCharacter::GetGameplayCameraComponent() const
{
	return GameplayCameraComponent;
}

UInputMappingContext* ARsPlayerCharacter::GetDefaultMappingContext() const
{
	return DefaultMappingContext;
}
