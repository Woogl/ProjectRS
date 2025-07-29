// Copyright 2025 Team BH.


#include "RsAnimNotify_LockOn.h"

#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotify_LockOn::URsAnimNotify_LockOn()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_LockOn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
#if WITH_EDITOR
	UWorld* World = MeshComp->GetWorld();
	if (World->WorldType == EWorldType::EditorPreview)
	{
		TArray<AActor*> OutActors;
		URsTargetingLibrary::PerformTargeting(Owner, Owner->GetTransform(), Shape, Collision, Filter, Sorter, OutActors);
	}
#endif // WITH_EDITOR
	
	APawn* Pawn = Cast<APawn>(Owner);
	if (!Pawn)
	{
		return;
	}

	if (AController* Controller = Pawn->GetController())
	{
		if (Controller->IsPlayerController())
		{
			// Player use manual lock on. (GA_LockOn)
			return;
		}
		if (URsLockOnComponent* LockOnComponent = Controller->FindComponentByClass<URsLockOnComponent>())
		{
			LockOnComponent->SetTargetingParams(Shape, Collision, Filter, Sorter);
			LockOnComponent->TryTargetingLockOn();
		}
	}
}
