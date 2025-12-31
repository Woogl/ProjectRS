// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsTargetingTypes.generated.h"

#define ECC_RsAttack ECollisionChannel::ECC_GameTraceChannel1

class URsConditionBase;

UENUM()
enum class ERsTargetingShapeType : uint8
{
	Box,
	Sphere,
	Capsule,
};

USTRUCT(BlueprintType)
struct FRsTargetingShape
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERsTargetingShapeType ShapeType = ERsTargetingShapeType::Sphere;
	
	UPROPERTY(EditAnywhere)
	FVector HalfExtent = FVector(50.f, 50.f, 50.f);
	
	UPROPERTY(EditAnywhere)
	FTransform ComponentOffset = FTransform::Identity;
	
	FCollisionShape MakeShape() const;
};

USTRUCT(BlueprintType)
struct FRsTargetingFilter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	bool bIncludeSelf = false;
	
	UPROPERTY(EditAnywhere)
	bool bIncludeFriendlyTeam = false;
	
	UPROPERTY(EditAnywhere)
	bool bIncludeHostileTeam = true;

	UPROPERTY(EditAnywhere)
	TObjectPtr<URsConditionBase> Condition;
	
	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToIgnore;
};

UENUM()
enum class ERsSortingOrder : uint8
{
	None,
	Ascending,
	Descending,
};

USTRUCT(BlueprintType)
struct FRsTargetingSorter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERsSortingOrder ByDistance = ERsSortingOrder::None;
};

USTRUCT(BlueprintType)
struct FRsTargetingParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FRsTargetingShape Shape;

	UPROPERTY(EditAnywhere)
	FRsTargetingFilter Filter;

	UPROPERTY(EditAnywhere)
	FRsTargetingSorter Sorter;

	FRsTargetingParams() {}
	FRsTargetingParams(FRsTargetingShape Shape, FRsTargetingFilter Filter, FRsTargetingSorter Sorter);
};