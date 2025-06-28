// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RsGhostTrail.generated.h"

class UPoseableMeshComponent;

UCLASS()
class RS_API ARsGhostTrail : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	UPoseableMeshComponent* PoseableMesh;
	
public:	
	ARsGhostTrail();
	void InitAppearance(USkeletalMeshComponent* OwnerComponent);

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	float LifeTime = 0.5f;
};
