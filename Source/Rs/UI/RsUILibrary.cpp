// Copyright 2024 Team BH.


#include "RsUILibrary.h"

#include "PrimaryGameLayout.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/RsUIManagerSubsystem.h"
#include "Widget/RsHUDLayout.h"

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

URsHUDLayout* URsUILibrary::GetGameHUD(UObject* WorldContextObject)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return GameInstance->GetSubsystem<URsUIManagerSubsystem>()->GetGameHUD();
	}
	return nullptr;
}

void URsUILibrary::AddSystemMessage(UObject* WorldContextObject, FText Message, float Duration)
{
	if (URsHUDLayout* GameHUD = GetGameHUD(WorldContextObject))
	{
		GameHUD->AddSystemMessage(Message, Duration);
	}
}
