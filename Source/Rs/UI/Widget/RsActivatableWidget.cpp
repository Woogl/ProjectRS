// Copyright 2024 Team BH.


#include "RsActivatableWidget.h"

#include "Input/CommonUIActionRouterBase.h"
#include "Rs/World/TimeControl/RsTimeControlLibrary.h"

void URsActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (APlayerController* Player = GetOwningPlayer())
	{
		if (PauseControl == ERsWidgetPauseMode::GamePause)
		{
			URsTimeControlLibrary::RequestTimePause(Player, GetFNameSafe(this), ERsTimeControlPriority::UI, BlendTime);
		}
		else if (PauseControl == ERsWidgetPauseMode::TimeDilation)
		{
			URsTimeControlLibrary::RequestTimeDilation(Player, GetFNameSafe(this), ERsTimeControlPriority::UI, TimeDilation, -1.f, BlendTime);
		}
	}
}

void URsActivatableWidget::NativeOnDeactivated()
{
	if (APlayerController* Player = GetOwningPlayer())
	{
		if (PauseControl == ERsWidgetPauseMode::GamePause)
		{
			URsTimeControlLibrary::RequestTimeResume(Player, GetFNameSafe(this), BlendTime);
		}
		else if (PauseControl == ERsWidgetPauseMode::TimeDilation)
		{
			URsTimeControlLibrary::RequestTimeResume(Player, GetFNameSafe(this), BlendTime);
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
