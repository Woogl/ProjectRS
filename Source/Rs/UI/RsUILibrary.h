// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsUILibrary.generated.h"

class UMVVMViewModelBase;
class UCommonActivatableWidget;
/**
 * 
 */
UCLASS()
class RS_API URsUILibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Rs UI Library", meta = (WorldContext = "WorldContextObject"))
	static void ShowGameHUD(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Rs UI Library", meta = (WorldContext = "WorldContextObject"))
	static void HideGameHUD(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Rs UI Library", meta = (WorldContext = "WorldContextObject"))
	static UCommonActivatableWidget* GetGameHUD(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="RS UI Library")
	static bool SetViewModelByName(UUserWidget* Widget, FName ViewModelName, UMVVMViewModelBase* ViewModel);

	UFUNCTION(BlueprintCallable, Category="RS UI Library")
	static bool SetViewModelByClass(UUserWidget* Widget, UMVVMViewModelBase* ViewModel);
};
