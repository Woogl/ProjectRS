// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_Targeting.generated.h"

UENUM()
enum class ERsTargetingShape : uint8
{
	Box,
	Cylinder,
	Sphere,
	Capsule,
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location", meta = (GetOptions="GetSocketNames"))
	FName SocketName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location")
	FVector Offset = FVector::ZeroVector;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool bShowDebugInPIE = false;

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FCollisionShape GetCollisionShape() const;
	
	UFUNCTION()
	TArray<FName> GetSocketNames() const;
	
	TWeakObjectPtr<USkeletalMeshComponent> CachedMeshComp;

#if WITH_EDITOR
	void DrawDebugShape(USkeletalMeshComponent* MeshComp, FTransform SourceTransform);
#endif // WITH_EDITOR
};
