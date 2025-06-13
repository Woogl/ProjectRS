// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_Teleport.generated.h"

UENUM()
enum class ERsTeleportPosition : uint8
{
	LocalPosition,
	WorldPosition
};
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_Teleport : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERsTeleportPosition PositionMode = ERsTeleportPosition::LocalPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLookTarget = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSearchDistance = 10000.f;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
