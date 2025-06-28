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
			ARsGhostTrail* SpawnedActor = Owner->GetWorld()->SpawnActor<ARsGhostTrail>(GhostTrailClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
			SpawnedActor->InitAppearance(Component);
			// Location and Rotation will be set in ARsGhostTrail::BeginPlay().
		}
	}
}
