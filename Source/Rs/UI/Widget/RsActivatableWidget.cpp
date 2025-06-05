// Copyright 2024 Team BH.


#include "RsActivatableWidget.h"

#include "Input/CommonUIActionRouterBase.h"
#include "Kismet/GameplayStatics.h"

void URsActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (PauseControl == ERsWidgetPauseMode::GamePause)
	{
		UGameplayStatics::SetGamePaused(GetOwningPlayer(), true);
	}
	else if (PauseControl == ERsWidgetPauseMode::GameResume)
	{
		UGameplayStatics::SetGamePaused(GetOwningPlayer(), false);
	}
}

void URsActivatableWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	if (PauseControl == ERsWidgetPauseMode::GamePause)
	{
		UGameplayStatics::SetGamePaused(GetOwningPlayer(), false);
	}
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
