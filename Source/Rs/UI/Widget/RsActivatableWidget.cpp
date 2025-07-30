// Copyright 2024 Team BH.


#include "RsActivatableWidget.h"

#include "Input/CommonUIActionRouterBase.h"
#include "Rs/World/RsWorldTimeLibrary.h"

void URsActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (APlayerController* Player = GetOwningPlayer())
	{
		if (PauseControl == ERsWidgetPauseMode::GamePause)
		{
			URsWorldTimeLibrary::RequestTimePause(Player, ERsTimeControlPriority::UI);
		}
		else if (PauseControl == ERsWidgetPauseMode::TimeDilation)
		{
			URsWorldTimeLibrary::RequestTimeDilation(Player, ERsTimeControlPriority::UI, TimeDilation);
		}
	}
}

void URsActivatableWidget::NativeOnDeactivated()
{
	if (APlayerController* Player = GetOwningPlayer())
	{
		if (PauseControl == ERsWidgetPauseMode::GamePause)
		{
			URsWorldTimeLibrary::RequestTimeResume(Player, ERsTimeControlPriority::UI);
		}
		else if (PauseControl == ERsWidgetPauseMode::TimeDilation)
		{
			URsWorldTimeLibrary::RequestTimeResume(Player, ERsTimeControlPriority::UI);
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
