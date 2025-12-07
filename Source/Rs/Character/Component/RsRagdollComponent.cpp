// Copyright 2025 Team BH.


#include "RsRagdollComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Rs/RsLogChannels.h"


// Sets default values for this component's properties
URsRagdollComponent::URsRagdollComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

void URsRagdollComponent::StartRagdoll_Replicated()
{
	if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}

	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		StartRagdoll_Multicast();
	}
	else
	{
		if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			Character->GetCharacterMovement()->FlushServerMoves();
		}
		StartRagdoll_Server();
	}
}

void URsRagdollComponent::StartRagdoll_Local()
{
	MeshComponent->bUpdateJointsFromAnimation = true; // Required for the flail animation to work properly.

	if (!MeshComponent->IsRunningParallelEvaluation() && !MeshComponent->GetBoneSpaceTransforms().IsEmpty())
	{
		MeshComponent->UpdateRBJointMotors();
	}

	// Stop any active montages.

	static constexpr auto BlendOutDuration{0.2f};

	MeshComponent->GetAnimInstance()->Montage_Stop(BlendOutDuration);

	if (IsValid(CharacterOwner))
	{
		// Disable movement corrections and reset network smoothing.
		CharacterOwner->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		CharacterOwner->GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = true;
	}

	// Detach the mesh so that character transformation changes will not affect it in any way.

	MeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	// Disable capsule collision and enable mesh physics simulation.

	if (UPrimitiveComponent* RootComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent()))
	{
		RootComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	MeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetSimulatePhysics(true);

	// This is required for the ragdoll to behave properly when any body instance is set to simulated in a physics asset.
	MeshComponent->ResetAllBodiesSimulatePhysics();

	// Clear the character movement mode and set the locomotion action to Ragdoll.

	if (IsValid(CharacterOwner))
	{
		CharacterOwner->GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
}

void URsRagdollComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = GetOwner<ACharacter>();

	if (CharacterOwner)
	{
		MeshComponent = CharacterOwner->GetMesh();
	}

	if (!MeshComponent)
	{
		MeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	}
	
	if (!MeshComponent)
	{
		UE_LOG(RsLog, Error, TEXT("Require skeletal mesh component for the Ragdoll to work."));
	}
}

void URsRagdollComponent::StartRagdoll_Multicast_Implementation()
{
	StartRagdoll_Local();
}

void URsRagdollComponent::StartRagdoll_Server_Implementation()
{
	StartRagdoll_Multicast();
	GetOwner()->ForceNetUpdate();
}



