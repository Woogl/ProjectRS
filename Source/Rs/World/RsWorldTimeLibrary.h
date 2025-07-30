// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsWorldTimeLibrary.generated.h"

UENUM(BlueprintType)
enum class ERsTimeControlPriority : uint8
{
	System,
	UI,
	VFX,
};

/**
 * 
 */
UCLASS()
class RS_API URsWorldTimeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Pause time with the specified priority.
	UFUNCTION(BlueprintCallable, Category = "RS World Time Library", meta = (WorldContext = "WorldContext"))
	static void RequestTimePause(UObject* WorldContext, ERsTimeControlPriority Priority);

	// Resume time by remove equal or below the given priority.
	UFUNCTION(BlueprintCallable, Category = "RS World Time Library", meta = (WorldContext = "WorldContext"))
	static void RequestTimeResume(UObject* WorldContext, ERsTimeControlPriority Priority);

	// Applies the highest-priority, most recent request.
	UFUNCTION(BlueprintCallable, Category = "RS World Time Library", meta = (WorldContext = "WorldContext"))
	static void RequestTimeDilation(UObject* WorldContext, ERsTimeControlPriority Priority, float Dilation, float Duration = -1.f);
};
