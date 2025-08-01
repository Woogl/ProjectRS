// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsTimeControlLibrary.generated.h"

UENUM(BlueprintType)
enum class ERsTimeControlPriority : uint8
{
	None	UMETA(Hidden),
	VFX,	// Lowest priority. Used for visual effects like combat slow-motion or cinematic pauses. 
	UI,		// Medium priority. Used for UI-driven time control.
	System,	// Highest priority. Used for critical gameplay or engine-level time control that must override others.
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
	static void RequestTimePause(UObject* WorldContext, FName RequestKey, ERsTimeControlPriority Priority, float BlendTime);

	// Time dilation. (The highest-priority, the latest request will apply.)
	UFUNCTION(BlueprintCallable, Category = "RS World Time Library", meta = (WorldContext = "WorldContext"))
	static void RequestTimeDilation(UObject* WorldContext, FName RequestKey, ERsTimeControlPriority Priority, float Dilation, float Duration, float BlendTime);

	// Resume time. (The highest-priority, the latest request will apply.)
	UFUNCTION(BlueprintCallable, Category = "RS World Time Library", meta = (WorldContext = "WorldContext"))
	static void RequestTimeResume(UObject* WorldContext, FName RequestKey, float BlendTime);
};
