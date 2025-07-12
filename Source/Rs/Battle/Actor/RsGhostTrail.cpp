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
		PoseableMesh->SetSkinnedAssetAndUpdate(SkeletalMesh);
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

#if WITH_EDITOR
void ARsGhostTrail::PostEditChangeProperty(FPropertyChangedEvent& InPropertyChangedEvent)
{
	Super::PostEditChangeProperty(InPropertyChangedEvent);

	const FName PropertyName = InPropertyChangedEvent.Property != nullptr ? InPropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, Material))
	{
		for (int32 i = 0; i < PoseableMesh->GetNumMaterials(); i++)
		{
			PoseableMesh->SetMaterial(i, Material);
		}
	}
}
#endif //WITH_EDITOR