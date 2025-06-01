// Copyright 2024 Team BH.


#include "RsActivatableWidget.h"

#include "Input/CommonUIActionRouterBase.h"

void URsActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void URsActivatableWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}

TOptional<FUIInputConfig> URsActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
		case ERsWidgetInputMode::GameAndMenu:
		{
			FUIInputConfig GameAndMenuConfig(ECommonInputMode::All, EMouseCaptureMode::CapturePermanently);
			return GameAndMenuConfig;
		}

		case ERsWidgetInputMode::Game:
		{
			FUIInputConfig GameConfig(ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently);
			GameConfig.bIgnoreLookInput = false;
			GameConfig.bIgnoreMoveInput = false;
			return GameConfig;
		}

		case ERsWidgetInputMode::Menu:
		{
			FUIInputConfig MenuConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
			MenuConfig.bIgnoreLookInput = true;
			MenuConfig.bIgnoreMoveInput = true;
			return MenuConfig;
		}

		case ERsWidgetInputMode::Default:
		default:
			return TOptional<FUIInputConfig>();
	}
}
