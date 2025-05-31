// Copyright 2024 Team BH.


#include "RsActivatableWidget.h"

#include "Input/CommonUIActionRouterBase.h"

void URsActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (InputConfig == ERsWidgetInputMode::Menu)
		{
			PlayerController->SetShowMouseCursor(true);
		}
		else if (InputConfig == ERsWidgetInputMode::Game)
		{
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void URsActivatableWidget::NativeOnDeactivated()
{
	if (UCommonUIActionRouterBase* UIActionRouter = ULocalPlayer::GetSubsystem<UCommonUIActionRouterBase>(GetOwningLocalPlayer()))
	{
		UIActionRouter->RefreshUIInputConfig();
	}
	
	Super::NativeOnDeactivated();
}

TOptional<FUIInputConfig> URsActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
		case ERsWidgetInputMode::GameAndMenu:
		{
			FUIInputConfig GameAndMenuConfig(ECommonInputMode::All, EMouseCaptureMode::NoCapture);
			return GameAndMenuConfig;
		}

		case ERsWidgetInputMode::Game:
		{
			FUIInputConfig GameConfig(ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown, false);
			GameConfig.bIgnoreLookInput = false;
			GameConfig.bIgnoreMoveInput = false;
			return GameConfig;
		}

		case ERsWidgetInputMode::Menu:
		{
			FUIInputConfig MenuConfig(ECommonInputMode::Menu, EMouseCaptureMode::CaptureDuringMouseDown, false);
			MenuConfig.bIgnoreLookInput = true;
			MenuConfig.bIgnoreMoveInput = true;
			return MenuConfig;
		}

		case ERsWidgetInputMode::Default:
		default:
			return TOptional<FUIInputConfig>();
	}
}
