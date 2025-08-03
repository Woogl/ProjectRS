// Copyright 2024 Team BH.


#include "RsActivatableWidget.h"

#include "Input/CommonUIActionRouterBase.h"
#include "Rs/World/TimeControl/RsTimeControlLibrary.h"

void URsActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (APlayerController* Player = GetOwningPlayer())
	{
		if (TimeControl == ERsWidgetTimeControl::GamePause)
		{
			URsTimeControlLibrary::RequestTimePause(Player, GetFNameSafe(this), ERsTimeControlPriority::UI, BlendTime);
		}
		else if (TimeControl == ERsWidgetTimeControl::TimeDilation)
		{
			URsTimeControlLibrary::RequestTimeDilation(Player, GetFNameSafe(this), ERsTimeControlPriority::UI, TimeDilation, -1.f, BlendTime);
		}
	}
}

void URsActivatableWidget::NativeOnDeactivated()
{
	if (APlayerController* Player = GetOwningPlayer())
	{
		if (TimeControl == ERsWidgetTimeControl::GamePause)
		{
			URsTimeControlLibrary::RequestTimeResume(Player, GetFNameSafe(this), BlendTime);
		}
		else if (TimeControl == ERsWidgetTimeControl::TimeDilation)
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
		case ERsWidgetInputMode::Game:
		{
			return FUIInputConfig(ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently);
		}
	
		case ERsWidgetInputMode::GameAndMenu:
		{
			FUIInputConfig Config(ECommonInputMode::All, EMouseCaptureMode::CapturePermanently);
			Config.bIgnoreLookInput = true;
			return Config;
		}

		case ERsWidgetInputMode::Menu:
		{
			FUIInputConfig Config(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
			Config.bIgnoreMoveInput = true;
			Config.bIgnoreLookInput = true;
			return Config;
		}

		case ERsWidgetInputMode::Default:
		default:
			return TOptional<FUIInputConfig>();
	}
}
