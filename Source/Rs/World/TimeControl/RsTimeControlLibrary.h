// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsTimeControlLibrary.generated.h"

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
class RS_API URsTimeControlLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Pause time. Time must be resumed by the caller! (The highest-priority, the latest request will apply.)
	UFUNCTION(BlueprintCallable, Category = "RS World Time Library", meta = (WorldContext = "WorldContext"))
	static void RequestTimePause(UObject* WorldContext, FName RequestKey, ERsTimeControlPriority Priority);

	// Time dilation. (The highest-priority, the latest request will apply.)
	UFUNCTION(BlueprintCallable, Category = "RS World Time Library", meta = (WorldContext = "WorldContext"))
	static void RequestTimeDilation(UObject* WorldContext, FName RequestKey, ERsTimeControlPriority Priority, float Dilation, float Duration = -1.f);

	// Resume time. (The highest-priority, the latest request will apply.)
	UFUNCTION(BlueprintCallable, Category = "RS World Time Library", meta = (WorldContext = "WorldContext"))
	static void RequestTimeResume(UObject* WorldContext, FName RequestKey);
};
