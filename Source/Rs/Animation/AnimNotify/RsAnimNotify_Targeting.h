// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsTargetingTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_Targeting.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_Targeting : public UAnimNotify
{
	GENERATED_BODY()

public:
	URsAnimNotify_Targeting();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay Event")
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Source", meta = (GetOptions="GetSocketNames"))
	FName SocketName = NAME_None;

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
	bool bPreventSelfDamage = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Filter")
	bool bPreventTeamDamage = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sorter")
	bool bSortByDistance = false;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool bShowDebugInPIE = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	float DebugLifeTime = 0.5f;
#endif // WITH_EDITORONLY_DATA

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> ResultActors;

	FCollisionShape GetCollisionShape() const;
	
#if WITH_EDITOR
	UFUNCTION()
	TArray<FName> GetSocketNames() const;
	
	void DrawDebugShape(USkeletalMeshComponent* MeshComp, FTransform SourceTransform);
#endif // WITH_EDITOR

#if WITH_EDITORONLY_DATA
	TWeakObjectPtr<USkeletalMeshComponent> CachedMeshComp;
#endif // WITH_EDITORONLY_DATA
};
