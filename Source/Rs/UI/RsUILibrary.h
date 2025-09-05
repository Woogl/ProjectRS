// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsUILibrary.generated.h"

class UCommonActivatableWidget;
class URsViewModelBase;
class URsActivatableWidget;
class URsHUDLayout;
/**
 * 
 */
UCLASS()
class RS_API URsUILibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** UI Layer */
	UFUNCTION(BlueprintCallable, Category = "RS UI Library", Meta = (GameplayTagFilter = "UI.Layer"))
	static UCommonActivatableWidget* GetActiveLayerWidget(ULocalPlayer* LocalPlayer, FGameplayTag Layer);
	
	UFUNCTION(BlueprintCallable, Category = "RS UI Library", Meta = (GameplayTagFilter = "UI.Layer"))
	static URsActivatableWidget* PushSceneWidgetToLayer(ULocalPlayer* LocalPlayer, FGameplayTag Layer, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<URsActivatableWidget> WidgetClass, TArray<URsViewModelBase*> ViewModels);

	UFUNCTION(BlueprintCallable, Category = "RS UI Library", Meta = (GameplayTagFilter = "UI.Layer"))
	static void PushSceneWidgetToLayerAsync(ULocalPlayer* LocalPlayer, FGameplayTag Layer, bool bSuspendInputUntilComplete, UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<URsActivatableWidget> SoftWidgetClass, TArray<URsViewModelBase*> ViewModels);
	
	/** MVVM */
	UFUNCTION(BlueprintCallable, Category = "RS UI Library")
	static bool SetViewModelByName(UUserWidget* Widget, FName Name, URsViewModelBase* ViewModel);
	
	UFUNCTION(BlueprintCallable, Category = "RS UI Library")
	static bool SetViewModelByClass(UUserWidget* Widget, URsViewModelBase* ViewModel);

	UFUNCTION(BlueprintCallable, Category = "RS UI Library")
	static URsViewModelBase* GetViewModelByName(UUserWidget* Widget, FName ViewModelName);

	UFUNCTION(BlueprintCallable, Category = "RS UI Library")
	static URsViewModelBase* GetViewModelByClass(UUserWidget* Widget, TSubclassOf<URsViewModelBase> ViewModelClass);

	/** Game HUD */
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
