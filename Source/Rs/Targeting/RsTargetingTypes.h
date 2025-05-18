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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HalfHeight = 100.0f;

	FCollisionShape MakeShape() const;

	FRsTargetingCollision() {}
	
	FRsTargetingCollision(TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes, ERsTargetingShape ShapeType, FVector HalfExtent, float Radius, float HalfHeight)
		: CollisionObjectTypes(CollisionObjectTypes), ShapeType(ShapeType), HalfExtent(HalfExtent), Radius(Radius), HalfHeight(HalfHeight)
	{}
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

	FRsTargetingFilter(bool bIncludeSelf, bool bIncludeFriendlyTeam, bool bIncludeHostileTeam, int32 MaxTargetCount, FGameplayTagRequirements TargetRequirements, TArray<AActor*> ActorsToIgnore)
		: bIncludeSelf(bIncludeSelf), bIncludeFriendlyTeam(bIncludeFriendlyTeam), bIncludeHostileTeam(bIncludeHostileTeam), MaxTargetCount(MaxTargetCount), TargetRequirements(TargetRequirements), ActorsToIgnore(ActorsToIgnore)
	{}
};

USTRUCT(BlueprintType)
struct FRsTargetingSorter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSortByDistance = false;

	FRsTargetingSorter() {}

	FRsTargetingSorter(bool bSortByDistance)
		: bSortByDistance(bSortByDistance)
	{}
};