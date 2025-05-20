// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Rs/Targeting/RsTargetingTypes.h"
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

	virtual FString GetNotifyName_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Source", meta = (GetOptions="GetSocketNames"))
	FName SocketName = FName("root");

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
	bool bIncludeSelf = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Filter")
	bool bIncludeFriendlyTeam = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Filter")
	bool bIncludeHostileTeam = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Filter")
	int32 MaxTargetCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Filter")
	FGameplayTagRequirements TargetRequirements;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sorter")
	bool bSortByDistance = false;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> ResultActors;

#if WITH_EDITOR
	UFUNCTION()
	TArray<FName> GetSocketNames() const { return SocketNames; }
#endif // WITH_EDITOR

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TArray<FName> SocketNames;
#endif // WITH_EDITORONLY_DATA
};
