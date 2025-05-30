// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "RsTargetingTypes.generated.h"

UENUM()
enum class ERsTargetingShape : uint8
{
	Box,
	Sphere,
	Capsule,
};

USTRUCT(BlueprintType)
struct FRsTargetingCollision
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERsTargetingShape ShapeType = ERsTargetingShape::Sphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HalfExtent = FVector(50.f, 50.f, 50.f);

	FCollisionShape MakeShape() const;

	FRsTargetingCollision() {}
	FRsTargetingCollision(TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes, ERsTargetingShape ShapeType, FVector HalfExtent);
};

USTRUCT(BlueprintType)
struct FRsTargetingFilter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIncludeSelf = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIncludeFriendlyTeam = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIncludeHostileTeam = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxTargetCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagRequirements TargetRequirements;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ActorsToIgnore;

	FRsTargetingFilter() {}
	FRsTargetingFilter(bool bIncludeSelf, bool bIncludeFriendlyTeam, bool bIncludeHostileTeam, int32 MaxTargetCount, FGameplayTagRequirements TargetRequirements, TArray<AActor*> ActorsToIgnore);
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
	ERsSortingOrder SortByDistance = ERsSortingOrder::None;

	FRsTargetingSorter() {}
	FRsTargetingSorter(ERsSortingOrder SortByDistance);
};