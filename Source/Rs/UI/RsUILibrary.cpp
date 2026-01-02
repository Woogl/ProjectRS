// Copyright 2024 Team BH.


#include "RsUILibrary.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "PrimaryGameLayout.h"
#include "Kismet/GameplayStatics.h"
#include "MVVM/ViewModel/RsViewModelBase.h"
#include "Rs/RsLogChannels.h"
#include "HUD/RsHUD.h"
#include "HUD/RsHUDLayout.h"
#include "Input/RsUIActionRouter.h"
#include "Rs/World/TimeControl/RsTimeControlLibrary.h"
#include "View/MVVMView.h"
#include "Widget/RsActivatableWidget.h"

UCommonActivatableWidget* URsUILibrary::GetActiveLayerWidget(ULocalPlayer* LocalPlayer, FGameplayTag Layer)
{
	if (UPrimaryGameLayout* PrimaryGameLayout = UPrimaryGameLayout::GetPrimaryGameLayout(LocalPlayer))
	{
		if (UCommonActivatableWidgetContainerBase* LayerWidget = PrimaryGameLayout->GetLayerWidget(Layer))
		{
			return LayerWidget->GetActiveWidget();
		}
	}
	return nullptr;
}

URsActivatableWidget* URsUILibrary::PushWidgetToLayer(ULocalPlayer* LocalPlayer, FGameplayTag Layer, TSubclassOf<URsActivatableWidget> WidgetClass, TArray<URsViewModelBase*> ViewModels)
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
					UE_LOG(LogRs, Warning, TEXT("Invalid ViewModel: %s in %s"), *ViewModel->GetClass()->GetName(), *WidgetClass->GetName());
				}
			}
		}
		return Cast<URsActivatableWidget>(SceneWidget);
	}
	return nullptr;
}

void URsUILibrary::PushWidgetToLayerAsync(ULocalPlayer* LocalPlayer, FGameplayTag Layer, bool bSuspendInputUntilComplete, TSoftClassPtr<URsActivatableWidget> SoftWidgetClass, TArray<URsViewModelBase*> ViewModels)
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
		return View->SetViewModel(ViewModelName, ViewModel);
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
		return View->SetViewModelByClass(ViewModel);
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
	if (const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (const ARsHUD* RsHUD = Cast<ARsHUD>(PlayerController->GetHUD()))
		{
			return RsHUD->GetGameHUD();
		}
	}
	return nullptr;
}

void URsUILibrary::OpenMenuWidget(UObject* WorldContextObject, FGameplayTag WidgetTag)
{
	if (const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARsHUD* RsHUD = Cast<ARsHUD>(PlayerController->GetHUD()))
		{
			RsHUD->OpenMenuWidget(WidgetTag);
		}
	}
}

void URsUILibrary::ShowMouseCursor(UObject* WorldContextObject)
{
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (URsUIActionRouter* UIActionRouter = PlayerController->GetLocalPlayer()->GetSubsystem<URsUIActionRouter>())
			{
				FUIInputConfig Config(ECommonInputMode::All, EMouseCaptureMode::NoCapture, false);
				Config.bIgnoreMoveInput = true;
				Config.bIgnoreLookInput = true;
				UIActionRouter->SetActiveUIInputConfig(Config);
			}
		}
	}
}

void URsUILibrary::ResetMouseCursor(UObject* WorldContextObject)
{
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (URsUIActionRouter* UIActionRouter = PlayerController->GetLocalPlayer()->GetSubsystem<URsUIActionRouter>())
			{
				UIActionRouter->ApplyLeafmostNodeConfig();
			}
		}
	}
}

void URsUILibrary::PrintSystemMessage(UObject* WorldContextObject, FText Message, float Duration)
{
	if (URsHUDLayout* GameHUD = GetGameHUD(WorldContextObject))
	{
		GameHUD->AddSystemMessage(Message, Duration);
	}
}

ULevelSequencePlayer* URsUILibrary::PlayCinematicSequence(UObject* WorldContextObject, ULevelSequence* LevelSequence, bool bHideGameHUD, float GlobalTimeDilation)
{
	// Avoid negative or divide-by-zero cases
	const float LocalGlobalTimeDilation = FMath::Max(GlobalTimeDilation, KINDA_SMALL_NUMBER);
	
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = true;
	Settings.PlayRate = 1.f / LocalGlobalTimeDilation;
	Settings.bDisableLookAtInput = true;
	Settings.bDisableMovementInput = true;
	
	ALevelSequenceActor* SpawnedActor; 
	if (ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(WorldContextObject, LevelSequence, Settings, SpawnedActor))
	{
		URsTimeControlLibrary::RequestTimeDilation(WorldContextObject, TEXT("PlayCinematicSequence"), ERsTimeControlPriority::System, LocalGlobalTimeDilation, 0, 0);
		if (bHideGameHUD)
		{
			HideGameHUD(WorldContextObject);
		}
		
		TWeakObjectPtr<ALevelSequenceActor> WeakActor = SpawnedActor;
		LevelSequencePlayer->OnNativeFinished.BindWeakLambda(SpawnedActor, [WorldContextObject, bHideGameHUD, WeakActor]()
		{
			URsTimeControlLibrary::RequestTimeResume(WorldContextObject, TEXT("PlayCinematicSequence"), 0.f);
			if (bHideGameHUD)
			{
				ShowGameHUD(WorldContextObject);
			}
			if (WeakActor.IsValid())
			{
				WeakActor->Destroy();
			}
		});
		
		return LevelSequencePlayer;
	}
	UE_LOG(LogRsUI, Warning, TEXT("Failed to PlayCinematicSequence()"));
	return nullptr;
}
