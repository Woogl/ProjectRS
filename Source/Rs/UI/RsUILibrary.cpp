// Copyright 2024 Team BH.


#include "RsUILibrary.h"

#include "PrimaryGameLayout.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Kismet/GameplayStatics.h"
#include "ViewModel/RsViewModelBase.h"
#include "Rs/RsLogChannels.h"
#include "Subsystem/RsUIManagerSubsystem.h"
#include "View/MVVMView.h"
#include "Widget/RsHUDLayout.h"

URsActivatableWidget* URsUILibrary::PushSceneWidgetToLayer(const ULocalPlayer* LocalPlayer, FGameplayTag Layer, TSubclassOf<URsActivatableWidget> WidgetClass, TArray<URsViewModelBase*> ViewModels)
{
	if (UCommonActivatableWidget* SceneWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Layer, WidgetClass))
	{
		for (URsViewModelBase* ViewModel : ViewModels)
		{
			if (ViewModel)
			{
				bool bSuccess = SetViewModelByClass(SceneWidget, ViewModel);
				if (!bSuccess)
				{
					UE_LOG(RsLog, Warning, TEXT("Invalid ViewModel: %s in %s"), *ViewModel->GetClass()->GetName(), *WidgetClass->GetName());
				}
			}
		}
		return Cast<URsActivatableWidget>(SceneWidget);
	}
	return nullptr;
}


bool URsUILibrary::SetViewModelByClass(UUserWidget* Widget, URsViewModelBase* ViewModel)
{
	if (!Widget)
	{
		return false;
	}
	if (UMVVMView* View = Cast<UMVVMView>(Widget->GetExtension<UMVVMView>()))
	{
		return View->SetViewModelByClass(ViewModel);
	}
	return false;
}

bool URsUILibrary::SetViewModelByName(UUserWidget* Widget, FName ViewModelName, URsViewModelBase* ViewModel)
{
	if (!Widget)
	{
		return false;
	}
	if (UMVVMView* View = Cast<UMVVMView>(Widget->GetExtension<UMVVMView>()))
	{
		return View->SetViewModel(ViewModelName, ViewModel);
	}
	return false;
}

URsViewModelBase* URsUILibrary::GetViewModel(UUserWidget* Widget, FName ViewModelName)
{
	if (!Widget)
	{
		return nullptr;
	}
	if (UMVVMView* View = Cast<UMVVMView>(Widget->GetExtension<UMVVMView>()))
	{
		if (TScriptInterface<INotifyFieldValueChanged> ViewModel = View->GetViewModel(ViewModelName))
		{
			return Cast<URsViewModelBase>(ViewModel.GetObject());
		}
	}
	return nullptr;
}

URsViewModelBase* URsUILibrary::GetViewModelByClass(UUserWidget* Widget, TSubclassOf<URsViewModelBase> ViewModelClass)
{
	if (!Widget)
	{
		return nullptr;
	}
	if (UMVVMView* View = Cast<UMVVMView>(Widget->GetExtension<UMVVMView>()))
	{
		const TArrayView<const FMVVMView_Source> ViewSources = View->GetSources();
		for (const FMVVMView_Source& ViewSource : ViewSources)
		{
			if (ViewSource.Source && ViewSource.Source.GetClass()->IsChildOf(ViewModelClass))
			{
				return Cast<URsViewModelBase>(ViewSource.Source);
			}
		}
	}
	return nullptr;
}

void URsUILibrary::ShowGameHUD(UObject* WorldContextObject, FGameplayTagContainer Layers)
{
	if (UPrimaryGameLayout* GameHUD = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject))
	{
		for (const FGameplayTag& Layer : Layers)
		{
			if (UCommonActivatableWidgetContainerBase* LayerWidget = GameHUD->GetLayerWidget(Layer))
			{
				if (UCommonActivatableWidget* ActiveWidget = LayerWidget->GetActiveWidget())
				{
					ActiveWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
		}
	}
}

void URsUILibrary::HideGameHUD(UObject* WorldContextObject, FGameplayTagContainer Layers)
{
	if (UPrimaryGameLayout* GameHUD = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject))
	{
		for (const FGameplayTag& Layer : Layers)
		{
			if (UCommonActivatableWidgetContainerBase* LayerWidget = GameHUD->GetLayerWidget(Layer))
			{
				if (UCommonActivatableWidget* ActiveWidget = LayerWidget->GetActiveWidget())
				{
					ActiveWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
}

URsHUDLayout* URsUILibrary::GetGameHUD(UObject* WorldContextObject)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return GameInstance->GetSubsystem<URsUIManagerSubsystem>()->GetGameHUD();
	}
	return nullptr;
}

void URsUILibrary::ShowCursor(UObject* WorldContextObject)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PlayerController)
	{
		return;
	}
	
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}
	
	if (UCommonUIActionRouterBase* UIActionRouter = LocalPlayer->GetSubsystem<UCommonUIActionRouterBase>())
	{
		FUIInputConfig Config(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
		Config.bIgnoreMoveInput = true;
		Config.bIgnoreLookInput = true;
		UIActionRouter->SetActiveUIInputConfig(Config);
	}
	PlayerController->bShowMouseCursor = true;
}

void URsUILibrary::HideCursor(UObject* WorldContextObject)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PlayerController)
	{
		return;
	}
	
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	if (UCommonUIActionRouterBase* UIActionRouter = LocalPlayer->GetSubsystem<UCommonUIActionRouterBase>())
	{
		FUIInputConfig InputConfig(ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown, true);
		InputConfig.bIgnoreLookInput = false;
		InputConfig.bIgnoreMoveInput = false;
		UIActionRouter->SetActiveUIInputConfig(InputConfig);
	}
	PlayerController->bShowMouseCursor = false;
}

void URsUILibrary::AddSystemMessage(UObject* WorldContextObject, FText Message, float Duration)
{
	if (URsHUDLayout* GameHUD = GetGameHUD(WorldContextObject))
	{
		GameHUD->AddSystemMessage(Message, Duration);
	}
}
