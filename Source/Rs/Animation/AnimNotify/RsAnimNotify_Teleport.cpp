// Copyright 2025 Team BH.


#include "RsAnimNotify_Teleport.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Player/RsPlayerController.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

void URsAnimNotify_Teleport::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	AActor* TeleportTarget = nullptr;
	
	if (ACharacter* Character = Cast<ACharacter>(Owner))
	{
		if (AController* Controller = Character->GetController())
		{
			// PlayerCharacter use LockOnComp target.
			if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(Controller))
			{
				if (URsLockOnComponent* LockOnComponent = RsPlayerController->GetLockOnComponent())
				{
					TeleportTarget = LockOnComponent->GetLockedOnTarget();
				}
			}
			
			// EnemyCharacter use AI BlackBoard target.
			else if (AAIController* AIController = Cast<AAIController>(Controller))
			{
				if (UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent())
				{
					if (UObject* BBObject = BBComponent->GetValueAsObject(TEXT("TargetActor")))
					{
						TeleportTarget = Cast<AActor>(BBObject);
					}
				}
			}
		}
	}

	// Find target by targeting system.
	if (bFallbackToTargeting == true && TeleportTarget == nullptr)
	{
		TArray<AActor*> OutActors;
		if (URsTargetingLibrary::PerformTargeting(MeshComp->GetOwner(), Owner->GetActorTransform(), FallbackCollision, FallbackFilter, FallbackSorter, OutActors))
		{
			TeleportTarget = OutActors[0];
		}
	}

	FVector TeleportDestination = Position;
	if (PositionMode == ERsTeleportPosition::WorldPosition)
	{
		// Teleport to World Position.
		Owner->SetActorLocation(TeleportDestination);
	}
	else if (PositionMode == ERsTeleportPosition::LocalPosition)
	{
		if (TeleportTarget == nullptr)
		{
			return;
		}
		FRotator TargetRotation = TeleportTarget->GetActorRotation();
		TeleportDestination = TeleportTarget->GetActorLocation() + TargetRotation.RotateVector(Position);
		// Teleport to Local Position.
		Owner->SetActorLocation(TeleportDestination);
	}
	
	if (bLookTarget == true)
	{
		FVector LookDirection;
		if (TeleportTarget)
		{
			LookDirection = TeleportTarget->GetActorLocation() - Owner->GetActorLocation();
		}
		else
		{
			LookDirection = TeleportDestination - Owner->GetActorLocation();
		}
		
		FRotator LookAtRotation = LookDirection.Rotation();
		FRotator OriginalRotation = Owner->GetActorRotation();
		Owner->SetActorRotation(FRotator(OriginalRotation.Pitch, LookAtRotation.Yaw, OriginalRotation.Roll));
	}
}
