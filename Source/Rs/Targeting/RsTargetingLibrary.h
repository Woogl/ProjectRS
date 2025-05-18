// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsTargetingTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsTargetingLibrary.generated.h"

class UTargetingPreset;

namespace RsTargetingGlobals
{
	extern bool bTargetingShowDebug;
	extern float TargetingDebugTime;
}

/**
 * 
 */
UCLASS()
class RS_API URsTargetingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (DefaultToSelf = "Owner"))
	static bool PerformTargeting(AActor* Owner, FVector StartLoc, FRotator StartRot, const FRsTargetingCollision& Collision, const FRsTargetingFilter& Filter, const FRsTargetingSorter& Sorter, TArray<AActor*>& ResultActors, bool bDrawDebug = false);

	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (DefaultToSelf = "Owner"))
	static bool PerformTargetingWithSubsteps(AActor* Owner, FTransform Start, FTransform End, int32 Substeps, const FRsTargetingCollision& Collision, const FRsTargetingFilter& Filter, const FRsTargetingSorter& Sorter, TArray<AActor*>& ResultActors, bool bDrawDebug = false);
	
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (WorldContext = "WorldContext"))
	static TArray<AActor*> PerformOverlapping(UObject* WorldContext, FVector StartLoc, FRotator StartRot, const FRsTargetingCollision& Collision);

	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (DefaultToSelf = "Owner"))
	static TArray<AActor*> PerformFiltering(const TArray<AActor*>& InActors, AActor* Owner, const FRsTargetingFilter& Filter);

	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (DefaultToSelf = "Owner"))
	static TArray<AActor*> PerformSorting(const TArray<AActor*>& InActors, AActor* Owner, const FRsTargetingSorter& Sorter);
	
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library", meta = (WorldContext = "WorldContext"))
	static void DrawDebugShape(UObject* WorldContext, FVector StartLoc, FRotator StartRot, const FRsTargetingCollision& Collision, FColor Color);
	
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library")
	static bool ExecuteTargetingPreset(AActor* SourceActor, const UTargetingPreset* TargetingPreset, TArray<AActor*>& ResultActors);

	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library")
	static bool ShouldDrawDebugShape(UWorld* World, bool bDrawDebug = false);
};
