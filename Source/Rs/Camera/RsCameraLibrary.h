// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsCameraLibrary.generated.h"

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
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "RS Camera Library", meta=(WorldContext="WorldContextObject"))
	static UGameplayCameraComponent* GetPlayerCameraComponent(const UObject* WorldContextObject);
};
