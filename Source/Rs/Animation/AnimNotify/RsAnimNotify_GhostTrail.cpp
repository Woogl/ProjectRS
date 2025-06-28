// Copyright 2025 Team BH.


#include "RsAnimNotify_GhostTrail.h"

#include "Rs/Battle/Actor/RsGhostTrail.h"

void URsAnimNotify_GhostTrail::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = Owner;

		TArray<USkeletalMeshComponent*> OutComponents;
		Owner->GetComponents(USkeletalMeshComponent::StaticClass(), OutComponents);
		
		for (USkeletalMeshComponent* Component : OutComponents)
		{
			SpawnedActor = Owner->GetWorld()->SpawnActorDeferred<ARsGhostTrail>(GhostTrailClass, FTransform::Identity, Owner);
			SpawnedActor->InitAppearance(Component);

			if (UWorld* World = MeshComp->GetWorld())
			{
				FTimerHandle TimerHandle;
				World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::DestroyGhostTrail, SpawnedActor->LifeTime);
			}

			// Location and Rotation will be set in ARsGhostTrail::BeginPlay().
			SpawnedActor->FinishSpawning(FTransform::Identity);
		}
	}
}

void URsAnimNotify_GhostTrail::DestroyGhostTrail()
{
	if (SpawnedActor)
	{
		SpawnedActor->Destroy();
	}
}
