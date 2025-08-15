// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCameraTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsCameraLibrary.generated.h"

struct FCameraAnimationParams;
class UCameraAnimationSequence;
class URsLockOnComponent;
class UGameplayCameraComponent;

/**
 * 
 */
UCLASS()
class RS_API URsCameraLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "RS Camera Library", meta=(WorldContext="WorldContextObject"))
	static UGameplayCameraComponent* GetPlayerCameraComponent(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "RS Camera Library", meta=(WorldContext="WorldContextObject"))
	static void SwitchCameraRig(const UObject* WorldContextObject, ERsCameraRig CameraRig);

	UFUNCTION(BlueprintCallable, Category = "RS Camera Library", meta=(WorldContext="WorldContextObject"))
	static void SwitchCharacterFacingMode(const UObject* WorldContextObject, ERsCharacterFacingMode FacingMode);
	
	UFUNCTION(BlueprintCallable, Category = "RS Camera Library", meta=(WorldContext="WorldContextObject"))
	static void PlayCameraAnimation(const UObject* WorldContextObject, UCameraAnimationSequence* CameraAnimation, FCameraAnimationParams Params);

	UFUNCTION(BlueprintCallable, Category = "RS Camera Library", meta=(WorldContext="WorldContextObject"))
	static void StopCameraAnimation(const UObject* WorldContextObject, UCameraAnimationSequence* CameraAnimation, bool bImmediate = false);
};
