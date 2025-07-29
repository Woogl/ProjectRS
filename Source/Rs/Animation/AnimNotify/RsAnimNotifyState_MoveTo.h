// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyMisc.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_MoveTo.generated.h"

class ARsCharacterBase;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_MoveTo : public UAnimNotifyState
{
	GENERATED_BODY()

public:
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
	FRsTargetingShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingCollision Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingFilter Filter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingSorter Sorter;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

protected:
	TWeakObjectPtr<AActor> Target;

	FVector StartLocation = FVector::ZeroVector;
	float AcceptableRadiusSquared = 0.f;

	float Duration = 1.f;
	float Elapsed = 0.f;

	bool bShouldMove = true;
	
	float GetNotifyProgress() const;
};