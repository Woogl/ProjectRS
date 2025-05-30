// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsUILibrary.generated.h"

class URsHUDLayout;
class UMVVMViewModelBase;
/**
 * 
 */
UCLASS()
class RS_API URsUILibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS UI Library", meta = (WorldContext = "WorldContextObject"))
	static void ShowGameHUD(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "RS UI Library", meta = (WorldContext = "WorldContextObject"))
	static void HideGameHUD(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "RS UI Library", meta = (WorldContext = "WorldContextObject"))
	static URsHUDLayout* GetGameHUD(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "RS UI Library", meta = (WorldContext = "WorldContextObject"))
	static void ShowCursor(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "RS UI Library", meta = (WorldContext = "WorldContextObject"))
	static void HideCursor(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "RS UI Library", meta = (WorldContext = "WorldContextObject"))
	static void AddSystemMessage(UObject* WorldContextObject, FText Message, float Duration = 2.f);
};
