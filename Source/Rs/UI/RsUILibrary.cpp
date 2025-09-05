// Copyright 2024 Team BH.


#include "RsUILibrary.h"

#include "PrimaryGameLayout.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Kismet/GameplayStatics.h"
#include "MVVM/ViewModel/RsViewModelBase.h"
#include "Rs/RsLogChannels.h"
#include "RsUIManagerSubsystem.h"
#include "View/MVVMView.h"
#include "Widget/RsHUDLayout.h"
#include "Widget/RsActivatableWidget.h"

URsActivatableWidget* URsUILibrary::PushSceneWidgetToLayer(ULocalPlayer* LocalPlayer, FGameplayTag Layer, TSubclassOf<URsActivatableWidget> WidgetClass, TArray<URsViewModelBase*> ViewModels)
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

void URsUILibrary::PushSceneWidgetToLayerAsync(ULocalPlayer* LocalPlayer, FGameplayTag Layer, bool bSuspendInputUntilComplete, TSoftClassPtr<URsActivatableWidget> SoftWidgetClass, TArray<URsViewModelBase*> ViewModels)
{
	if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayout(LocalPlayer))
	{
		RootLayout->PushWidgetToLayerStackAsync<URsActivatableWidget>(Layer, bSuspendInputUntilComplete, SoftWidgetClass, [ViewModels](EAsyncWidgetLayerState State, URsActivatableWidget* Widget)
		{
			if (!Widget)
			{
				return;
			}
			switch (State)
			{
			case EAsyncWidgetLayerState::Initialize:
				for (URsViewModelBase* ViewModel : ViewModels)
				{
					SetViewModelByClass(Widget, ViewModel);
				}
				break;
			case EAsyncWidgetLayerState::AfterPush:
				break;
			case EAsyncWidgetLayerState::Canceled:
				break;
			}
		});
	}
}

bool URsUILibrary::SetViewModelByName(UUserWidget* Widget, FName ViewModelName, URsViewModelBase* ViewModel)
{
	if (!Widget)
	{
		return false;
	}
	if (UMVVMView* View = Cast<UMVVMView>(Widget->GetExtension<UMVVMView>()))
	{
		bool bSuccess = View->SetViewModel(ViewModelName, ViewModel);
		if (bSuccess)
		{
			if (UBlueprintGeneratedClass* WBPClass = Cast<UBlueprintGeneratedClass>(Widget->GetClass()))
			{
				WBPClass->BindDynamicDelegates(WBPClass, Widget);
			}
		}
		return bSuccess;
	}
	return false;
}

bool URsUILibrary::SetViewModelByClass(UUserWidget* Widget, URsViewModelBase* ViewModel)
{
	if (!Widget)
	{
		return false;
	}
	if (UMVVMView* View = Cast<UMVVMView>(Widget->GetExtension<UMVVMView>()))
	{
		bool bSuccess = View->SetViewModelByClass(ViewModel);
		if (bSuccess)
		{
			if (UBlueprintGeneratedClass* WBPClass = Cast<UBlueprintGeneratedClass>(Widget->GetClass()))
			{
				WBPClass->BindDynamicDelegates(WBPClass, Widget);
			}
		}
		return bSuccess;
	}
	return false;
}

URsViewModelBase* URsUILibrary::GetViewModelByName(UUserWidget* Widget, FName ViewModelName)
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

void URsUILibrary::ShowGameHUD(UObject* WorldContextObject)
{
	if (URsHUDLayout* GameHUD = GetGameHUD(WorldContextObject))
	{
		GameHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void URsUILibrary::HideGameHUD(UObject* WorldContextObject)
{
	if (URsHUDLayout* GameHUD = GetGameHUD(WorldContextObject))
	{
		GameHUD->SetVisibility(ESlateVisibility::Hidden);
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
