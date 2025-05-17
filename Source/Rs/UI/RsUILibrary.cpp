// Copyright 2024 Team BH.


#include "RsUILibrary.h"

#include "MVVMViewModelBase.h"
#include "PrimaryGameLayout.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/RsUIManagerSubsystem.h"
#include "View/MVVMView.h"

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

UCommonActivatableWidget* URsUILibrary::GetGameHUD(UObject* WorldContextObject)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return GameInstance->GetSubsystem<URsUIManagerSubsystem>()->GetGameHUD();
	}
	return nullptr;
}

bool URsUILibrary::SetViewModelByName(UUserWidget* Widget, FName ViewModelName, UMVVMViewModelBase* ViewModel)
{
	if (!Widget)
	{
		return false;
	}
	
	if (UMVVMView* View = Widget->GetExtension<UMVVMView>())
	{
		return View->SetViewModel(ViewModelName, ViewModel);
	}
	
	return false;
}

bool URsUILibrary::SetViewModelByClass(UUserWidget* Widget, UMVVMViewModelBase* ViewModel)
{
	if (!Widget)
	{
		return false;
	}
	
	if (UMVVMView* View = Widget->GetExtension<UMVVMView>())
	{
		View->SetViewModelByClass(ViewModel);
	}
	
	return false;
}
