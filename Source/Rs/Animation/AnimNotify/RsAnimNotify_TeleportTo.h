// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyMisc.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotify_TeleportTo.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_TeleportTo : public UAnimNotify
{
	GENERATED_BODY()

public:
	URsAnimNotify_TeleportTo();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERsPositionMode PositionMode = ERsPositionMode::TowardTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "PositionMode != ERsPositionMode::TowardTarget"))
	FVector Position = FVector::ZeroVector;

	// 0 or negative means no limit
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMoveDistance = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AcceptableRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLookTarget = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "PositionMode == ERsPositionMode::TargetLocalPosition || PositionMode == ERsPositionMode::TowardTarget"))
	bool bFallbackToTargeting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting && PositionMode == ERsPositionMode::TargetLocalPosition"))
	FRsTargetingCollision FallbackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting && PositionMode == ERsPositionMode::TargetLocalPosition"))
	FRsTargetingFilter FallbackFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting && PositionMode == ERsPositionMode::TargetLocalPosition"))
	FRsTargetingSorter FallbackSorter;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	AActor* FindTeleportTarget(AActor* Owner) const;
};
