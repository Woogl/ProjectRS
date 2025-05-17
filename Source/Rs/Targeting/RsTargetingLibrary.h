// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION(BlueprintCallable, Category = "RS Targeting Library")
	static bool ExecuteTargetingPreset(AActor* SourceActor, const UTargetingPreset* TargetingPreset, TArray<AActor*>& ResultActors);
};
