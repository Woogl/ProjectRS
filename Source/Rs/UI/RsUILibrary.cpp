// Copyright 2024 Team BH.


#include "RsUILibrary.h"

#include "PrimaryGameLayout.h"

void URsUILibrary::ShowGameHUD(UObject* WorldContextObject)
{
	if (UPrimaryGameLayout* GameHUD = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject))
	{
		GameHUD->SetVisibility(ESlateVisibility::Visible);
	}
}

void URsUILibrary::HideGameHUD(UObject* WorldContextObject)
{
	if (UPrimaryGameLayout* GameHUD = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject))
	{
		GameHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}
