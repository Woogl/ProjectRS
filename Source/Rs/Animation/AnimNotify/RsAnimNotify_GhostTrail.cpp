// Copyright 2025 Team BH.


#include "RsAnimNotify_GhostTrail.h"

#include "Rs/Battle/Actor/RsGhostTrail.h"

void URsAnimNotify_GhostTrail::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	SpawnedGhostTrail = nullptr;
	if (AActor* Owner = MeshComp->GetOwner())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = Owner;

		TArray<USkeletalMeshComponent*> OutComponents;
		Owner->GetComponents(USkeletalMeshComponent::StaticClass(), OutComponents);
		
		for (USkeletalMeshComponent* Component : OutComponents)
		{
			if (Component->IsVisible() == false)
			{
				continue;
			}
			
			SpawnedGhostTrail = Owner->GetWorld()->SpawnActorDeferred<ARsGhostTrail>(GhostTrailClass, FTransform::Identity, Owner);
			if (SpawnedGhostTrail)
			{
				SpawnedGhostTrail->InitAppearance(Component);
#if WITH_EDITOR
				// Prevent accumulate in EditorPreview.
				if (UWorld* World = MeshComp->GetWorld())
				{
					FTimerHandle TimerHandle;
					World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::DestroyGhostTrail, SpawnedGhostTrail->LifeTime);
				}
#endif //WITH_EDITOR
				// Location and Rotation will be set in ARsGhostTrail::BeginPlay().
				SpawnedGhostTrail->FinishSpawning(FTransform::Identity);
			}
		}
	}
}

void URsAnimNotify_GhostTrail::DestroyGhostTrail()
{
	if (SpawnedGhostTrail)
	{
		SpawnedGhostTrail->Destroy();
	}
}
