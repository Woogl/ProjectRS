// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "RsTargetingTypes.generated.h"

#define ECC_RsAttack ECollisionChannel::ECC_GameTraceChannel1

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERsTargetingShapeType ShapeType = ERsTargetingShapeType::Sphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HalfExtent = FVector(50.f, 50.f, 50.f);
	
	FCollisionShape MakeShape() const;

	FRsTargetingShape() {}
	FRsTargetingShape(ERsTargetingShapeType ShapeType, FVector HalfExtent);
};

USTRUCT(BlueprintType)
struct FRsTargetingCollision
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannels;
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
	FGameplayTagRequirements TargetRequirements;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ActorsToIgnore;

	FRsTargetingFilter() {}
	FRsTargetingFilter(bool bIncludeSelf, bool bIncludeFriendlyTeam, bool bIncludeHostileTeam, FGameplayTagRequirements TargetRequirements, TArray<AActor*> ActorsToIgnore);
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