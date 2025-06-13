// Copyright 2025 Team BH.


#include "RsAnimNotify_Teleport.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Character/RsEnemyCharacter.h"
#include "Rs/Player/RsPlayerController.h"

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

					// If the lock on target is none, Search the closest enemy.
					if (TeleportTarget == nullptr)
					{
						TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
						ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
						TArray<AActor*> OutActors;
						UKismetSystemLibrary::SphereOverlapActors(Owner->GetWorld(), Owner->GetActorLocation(), MaxSearchDistance, ObjectTypes, ARsEnemyCharacter::StaticClass(), TArray<AActor*>(), OutActors);

						const FVector OwnerLocation = Owner->GetActorLocation();
						float MinDistanceSquared = MaxSearchDistance * MaxSearchDistance;
						for (AActor* Actor : OutActors)
						{
							float DistanceSquared = FVector::DistSquared(Actor->GetActorLocation(), OwnerLocation);
							if (DistanceSquared < MinDistanceSquared)
							{
								MinDistanceSquared = DistanceSquared;
								TeleportTarget = Actor;
							}
						}
					}
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
