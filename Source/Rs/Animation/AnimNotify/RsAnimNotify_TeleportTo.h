// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyMisc.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotify_TeleportTo.generated.h"

class ARsCharacterBase;
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "PositionMode != ERsPositionMode::TowardTarget", EditConditionHides))
	FVector Position = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "PositionMode == ERsPositionMode::TowardTarget", EditConditionHides))
	float AcceptableRadius = 0.f;

	// 0 or negative means no limit
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMoveDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	bool bLookTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingCollision Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingFilter Filter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingSorter Sorter;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
