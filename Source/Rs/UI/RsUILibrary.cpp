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

URsActivatableWidget* URsUILibrary::PushSceneWidgetToLayer(const ULocalPlayer* LocalPlayer, FGameplayTag Layer, TSubclassOf<URsActivatableWidget> WidgetClass, TMap<FName, URsViewModelBase*> ViewModels)
{
	if (UCommonActivatableWidget* SceneWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Layer, WidgetClass))
	{
		if (UMVVMView* View = Cast<UMVVMView>(SceneWidget->GetExtension<UMVVMView>()))
		{
			for (const auto& [ViewModelName, ViewModelObject] : ViewModels)
			{
				bool bSuccess = View->SetViewModel(ViewModelName, ViewModelObject);
				if (!bSuccess)
				{
					UE_LOG(RsLog, Warning, TEXT("Invalid ViewModel: %s in %s"), *ViewModelName.ToString(), *WidgetClass->GetName());
				}
			}
		}
		return Cast<URsActivatableWidget>(SceneWidget);
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
		FUIInputConfig InputConfig(UIActionRouter->GetActiveInputMode(), EMouseCaptureMode::CaptureDuringMouseDown, false);
		InputConfig.bIgnoreLookInput = true;
		InputConfig.bIgnoreMoveInput = true;
		UIActionRouter->SetActiveUIInputConfig(InputConfig);
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
