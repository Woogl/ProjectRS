// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsTargetingTypes.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RsAnimNotifyState_Targeting.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_Targeting : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_Targeting();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Source", meta = (GetOptions="GetSocketNames"))
	FName SocketName = FName("pelvis");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Source")
	FVector PositionOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Source")
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shape")
	ERsTargetingShape ShapeType = ERsTargetingShape::Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shape", meta=(EditCondition = "ShapeType == ERsTargetingShape::Box || ShapeType == ERsTargetingShape::Cylinder", EditConditionHides))
	FVector HalfExtent = FVector(50.f, 50.f, 50.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shape", meta=(EditCondition = "ShapeType == ERsTargetingShape::Sphere || ShapeType == ERsTargetingShape::Capsule", EditConditionHides))
	float Radius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shape", meta=(EditCondition = "ShapeType == ERsTargetingShape::Capsule", EditConditionHides))
	float HalfHeight = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Filter")
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Filter")
	bool bIgnoreFriendlyTeam = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Filter")
	bool bIgnoreEnemyTeam = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sorter")
	bool bSortByDistance = true;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool bShowDebugInPIE = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	float DebugLifeTime = 0.5f;
#endif // WITH_EDITORONLY_DATA

protected:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
	bool PerformTargeting(const USkeletalMeshComponent* MeshComp, FTransform SourceTransform);
	TArray<TWeakObjectPtr<AActor>> Targets;
	
	FCollisionShape GetCollisionShape() const;

#if WITH_EDITOR
	UFUNCTION()
	TArray<FName> GetSocketNames() const { return SocketNames; }
	
	void DrawDebugShape(const UWorld* World, FTransform SourceTransform);
#endif // WITH_EDITOR

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TArray<FName> SocketNames;
#endif // WITH_EDITORONLY_DATA
};
