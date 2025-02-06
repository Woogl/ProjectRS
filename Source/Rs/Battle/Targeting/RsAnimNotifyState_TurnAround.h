// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsTargetingTypes.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RsAnimNotifyState_TurnAround.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_TurnAround : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_TurnAround();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location", meta = (GetOptions="GetSocketNames"))
	FName SocketName = FName("pelvis");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location")
	FVector Offset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Speed")
	float MaxRotatingSpeed = 300.f;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool bShowDebugInPIE = false;

private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	FCollisionShape GetCollisionShape() const;
	
	UFUNCTION()
	TArray<FName> GetSocketNames() const;
	
	TWeakObjectPtr<USkeletalMeshComponent> CachedMeshComp;

	TWeakObjectPtr<AActor> CachedTarget;

	UPROPERTY(Transient)
	bool bTurnComplete = false;

#if WITH_EDITOR
	void DrawDebugShape(USkeletalMeshComponent* MeshComp, FTransform SourceTransform);
#endif // WITH_EDITOR
};
