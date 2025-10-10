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
	static UGameplayCameraComponent* GetPlayerCameraComponent(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "RS Camera Library", meta=(WorldContext="WorldContextObject"))
	static void SetCameraRig(const UObject* WorldContextObject, ERsCameraRig CameraRig);
};
