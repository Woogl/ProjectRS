// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/BlueprintCameraVariableTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsCameraLibrary.generated.h"

enum class ERsCameraMode : uint8;
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

	UFUNCTION(BlueprintCallable, Category = "RS Camera Library", meta=(WorldContext="WorldContextObject"))
	static void SwitchCameraMode(const UObject* WorldContextObject, ERsCameraMode NewCameraMode);

	UFUNCTION(BlueprintCallable, Category = "RS Camera Library", meta=(WorldContext="WorldContextObject"))
	FBlueprintCameraVariableTable GetCameraVariableTable(const UObject* WorldContextObject);
};
