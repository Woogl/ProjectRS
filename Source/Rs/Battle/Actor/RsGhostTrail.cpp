// Copyright 2025 Team BH.


#include "RsGhostTrail.h"

#include "Components/PoseableMeshComponent.h"

ARsGhostTrail::ARsGhostTrail()
{
	PrimaryActorTick.bCanEverTick = false;

	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	SetRootComponent(PoseableMesh);
}

void ARsGhostTrail::InitAppearance(USkeletalMeshComponent* OwnerComponent)
{
	if (USkeletalMesh* SkeletalMesh = OwnerComponent->GetSkeletalMeshAsset(); OwnerComponent->IsVisible())
	{
		PoseableMesh->SetSkinnedAsset(SkeletalMesh);
		PoseableMesh->CopyPoseFromSkeletalComponent(OwnerComponent);
		PoseableMesh->SetWorldTransform(OwnerComponent->GetComponentTransform());
	}
}

void ARsGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime);

	if (GetOwner() && PoseableMesh && Material)
	{
		for (int32 i = 0; i < PoseableMesh->GetNumMaterials(); i++)
		{
			PoseableMesh->SetMaterial(i, Material);
		}
	}
}
