// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCameraTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsCameraLibrary.generated.h"

class ARsPlayerController;
class ARsCameraAnimationActor;
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
	static void SetCameraRig(const UObject* WorldContextObject, ERsCameraRig CameraRig);

	UFUNCTION(BlueprintCallable, Category = "RS Camera Library", meta=(WorldContext="WorldContextObject"))
	static void SetCharacterFacingMode(const UObject* WorldContextObject, ERsCharacterFacingMode FacingMode);
	
	UFUNCTION(BlueprintCallable, Category = "RS Camera Library")
	static ARsCameraAnimationActor* PlayCameraAnimationSequence(ARsPlayerController* PlayerController, UCameraAnimationSequence* Sequence, FCameraAnimationParams Params);

	UFUNCTION(BlueprintCallable, Category = "RS Camera Library")
	static void StopCameraAnimationSequence(ARsPlayerController* PlayerController, UCameraAnimationSequence* Sequence);
};
