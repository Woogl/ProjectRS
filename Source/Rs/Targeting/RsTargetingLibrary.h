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
	static bool PerformTargeting(const AActor* Owner, FTransform Transform, const FRsTargetingParams& Params, TArray<AActor*>& ResultActors, bool bDrawDebug = false);

	// Used in anim notifies.
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library")
	static bool PerformTargetingInMeshSpace(const UMeshComponent* MeshComp, const FRsTargetingParams& Params, TArray<AActor*>& ResultActors, bool bDrawDebug = false);
	
	// Performs multi-step targeting from Start to End to fill the gaps.
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (DefaultToSelf = "Owner"))
	static bool PerformTargetingWithSubsteps(const AActor* Owner, FTransform Start, FTransform End, int32 MaxSubsteps, const FRsTargetingParams& Params, TArray<AActor*>& ResultActors, bool bDrawDebug = false);
	
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (WorldContext = "WorldContext"))
	static TArray<AActor*> PerformOverlapping(const UObject* WorldContext, FTransform Transform, const FRsTargetingShape& Shape, bool bDrawDebug = false);

	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (DefaultToSelf = "Owner"))
	static TArray<AActor*> PerformFiltering(const TArray<AActor*>& InActors, const AActor* Owner, const FRsTargetingFilter& Filter);

	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (DefaultToSelf = "Owner"))
	static TArray<AActor*> PerformSorting(const TArray<AActor*>& InActors, const AActor* Owner, const FRsTargetingSorter& Sorter);
	
	// Use gameplay targeting system plugin.
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library")
	static bool ExecuteTargetingPreset(AActor* SourceActor, const UTargetingPreset* TargetingPreset, TArray<AActor*>& ResultActors);
	
	static void DrawDebugShape(const UWorld* World, const FTransform& Transform, const FRsTargetingShape& Shape, FColor Color);
	static void DrawDebugArrow(const UWorld* World, const FVector& Start, const FVector& End, FColor Color);
	static FTransform GetSocketWorldTransform(const USceneComponent* Component, FName SocketName = NAME_None, const FTransform& LocalOffset = FTransform::Identity);

private:
	static bool ShouldDrawDebug(const UWorld* World);
};
