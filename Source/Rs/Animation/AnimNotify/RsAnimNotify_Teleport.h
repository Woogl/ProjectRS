// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Rs/Targeting/RsTargetingTypes.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback")
	bool bFallbackToTargeting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting"))
	FRsTargetingCollision FallbackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting"))
	FRsTargetingFilter FallbackFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting"))
	FRsTargetingSorter FallbackSorter;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
