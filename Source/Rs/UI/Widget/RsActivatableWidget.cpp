// Copyright 2024 Team BH.


#include "RsActivatableWidget.h"

#include "Input/CommonUIActionRouterBase.h"
#include "Kismet/GameplayStatics.h"

void URsActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (APlayerController* Player = GetOwningPlayer())
	{
		if (PauseControl == ERsWidgetPauseMode::GamePause)
		{
			UGameplayStatics::SetGamePaused(Player, true);
		}
		else if (PauseControl == ERsWidgetPauseMode::TimeDilation)
		{
			UGameplayStatics::SetGlobalTimeDilation(Player, TimeDilation);
		}
	}
}

void URsActivatableWidget::NativeOnDeactivated()
{
	if (APlayerController* Player = GetOwningPlayer())
	{
		if (PauseControl == ERsWidgetPauseMode::GamePause)
		{
			UGameplayStatics::SetGamePaused(Player, false);
		}
		else if (PauseControl == ERsWidgetPauseMode::TimeDilation)
		{
			UGameplayStatics::SetGlobalTimeDilation(Player, 1.f);
		}
	}
	
	Super::NativeOnDeactivated();
}

TOptional<FUIInputConfig> URsActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
		case ERsWidgetInputMode::GameAndMenu:
		{
			return FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::CapturePermanently);
		}

		case ERsWidgetInputMode::Game:
		{
			return FUIInputConfig(ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently);
		}

		case ERsWidgetInputMode::Menu:
		{
			return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
		}

		case ERsWidgetInputMode::Default:
		default:
			return TOptional<FUIInputConfig>();
	}
}
