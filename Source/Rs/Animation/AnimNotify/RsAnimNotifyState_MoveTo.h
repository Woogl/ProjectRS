// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_MoveTo.generated.h"

UENUM()
enum class ERsMoveToPosition : uint8
{
	SourceLocalPosition,
	TargetLocalPosition,
	WorldPosition
};

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_MoveTo : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERsMoveToPosition PositionMode = ERsMoveToPosition::TargetLocalPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "PositionMode == ERsMoveToPosition::TargetLocalPosition"))
	bool bFallbackToTargeting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting && PositionMode == ERsMoveToPosition::TargetLocalPosition"))
	FRsTargetingCollision FallbackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting && PositionMode == ERsMoveToPosition::TargetLocalPosition"))
	FRsTargetingFilter FallbackFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting && PositionMode == ERsMoveToPosition::TargetLocalPosition"))
	FRsTargetingSorter FallbackSorter;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

protected:
	AActor* FindMoveTarget(AActor* Owner) const;
	
	FVector TargetLocation = FVector::ZeroVector;
};
