// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsTimeControlLibrary.generated.h"

UENUM(BlueprintType)
enum class ERsTimeControlPriority : uint8
{
	None	UMETA(Hidden),
	VFX		UMETA(Tooltip = "Lowest priority. Used for visual effects like combat slow-motion or cinematic pauses."), 
	UI		UMETA(Tooltip = "Medium priority. Used for UI-driven time control."),
	System	UMETA(Tooltip = "Highest priority. Used for critical gameplay or engine-level time control that must override others."),
};

/**
 * The highest-priority, the latest request will apply.
 */
UCLASS()
class RS_API URsTimeControlLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Must be resumed by the caller!
	UFUNCTION(BlueprintCallable, Category = "RS World Time Library", meta = (WorldContext = "WorldContext"))
	static void RequestTimePause(UObject* WorldContext, FName Key, ERsTimeControlPriority Priority, float BlendTime);

	// Duration 0 means infinite duration.
	UFUNCTION(BlueprintCallable, Category = "RS World Time Library", meta = (WorldContext = "WorldContext"))
	static void RequestTimeDilation(UObject* WorldContext, FName Key, ERsTimeControlPriority Priority, float Dilation, float Duration, float BlendTime);

	UFUNCTION(BlueprintCallable, Category = "RS World Time Library", meta = (WorldContext = "WorldContext"))
	static void RequestTimeResume(UObject* WorldContext, FName Key, float BlendTime);
};
