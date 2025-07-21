// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotify_Teleport.generated.h"

UENUM()
enum class ERsTeleportPosition : uint8
{
	SourceLocalPosition,
	TargetLocalPosition,
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
	URsAnimNotify_Teleport();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERsTeleportPosition PositionMode = ERsTeleportPosition::TargetLocalPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLookTarget = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "PositionMode == ERsTeleportPosition::TargetLocalPosition"))
	bool bFallbackToTargeting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting && PositionMode == ERsTeleportPosition::TargetLocalPosition"))
	FRsTargetingCollision FallbackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting && PositionMode == ERsTeleportPosition::TargetLocalPosition"))
	FRsTargetingFilter FallbackFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fallback", meta = (EditCondition = "bFallbackToTargeting && PositionMode == ERsTeleportPosition::TargetLocalPosition"))
	FRsTargetingSorter FallbackSorter;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	AActor* FindTeleportTarget(AActor* Owner) const;
};
