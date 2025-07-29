// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsTargetingTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsTargetingLibrary.generated.h"

class UTargetingPreset;

/**
 * 
 */
UCLASS()
class RS_API URsTargetingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Perform overlapping, filtering and sorting.
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (DefaultToSelf = "Owner"))
	static bool PerformTargeting(AActor* Owner, FTransform Transform, const FRsTargetingShape& Shape, const FRsTargetingCollision& Collision, FRsTargetingFilter Filter, FRsTargetingSorter Sorter, TArray<AActor*>& ResultActors, bool bDrawDebug = false);

	// Performs multi-step targeting from Start to End to fill the gaps.
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (DefaultToSelf = "Owner"))
	static bool PerformTargetingWithSubsteps(AActor* Owner, FTransform Start, FTransform End, int32 MaxSubsteps, const FRsTargetingShape& Shape, const FRsTargetingCollision& Collision, FRsTargetingFilter Filter, FRsTargetingSorter Sorter, TArray<AActor*>& ResultActors, bool bDrawDebug = false);
	
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (WorldContext = "WorldContext"))
	static TArray<AActor*> PerformOverlapping(UObject* WorldContext, FTransform Transform, const FRsTargetingShape& Shape, const FRsTargetingCollision& Collision);

	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (DefaultToSelf = "Owner"))
	static TArray<AActor*> PerformFiltering(const TArray<AActor*>& InActors, AActor* Owner, const FRsTargetingFilter& Filter);

	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (DefaultToSelf = "Owner"))
	static TArray<AActor*> PerformSorting(const TArray<AActor*>& InActors, AActor* Owner, const FRsTargetingSorter& Sorter);
	
	// Use gameplay targeting system plugin.
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library")
	static bool ExecuteTargetingPreset(AActor* SourceActor, const UTargetingPreset* TargetingPreset, TArray<AActor*>& ResultActors);

	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (WorldContext = "WorldContext"))
	static void DrawDebugShape(UObject* WorldContext, FTransform Transform, const FRsTargetingShape& Shape, const FRsTargetingCollision& Collision, FColor Color);
};
