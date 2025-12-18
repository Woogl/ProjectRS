// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCameraTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsCameraLibrary.generated.h"

class ARsPlayerController;
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
	static UGameplayCameraComponent* GetPlayerGameplayCameraComponent(const UObject* WorldContextObject, int32 PlayerIndex);
	
	UFUNCTION(BlueprintCallable, Category = "RS Camera Library", meta=(WorldContext="WorldContextObject"))
	static void SwitchPlayerCameraRig(const UObject* WorldContextObject, ERsCameraRig CameraRig);
};
