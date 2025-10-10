// Copyright 2025 Team BH.


#include "RsBattleViewModelResolver.h"

#include "Blueprint/UserWidget.h"
#include "Rs/UI/MVVM/ViewModel/RsBattleViewModel.h"

UObject* URsBattleViewModelResolver::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const
{
	ULocalPlayer* LocalPlayer = UserWidget->GetOwningLocalPlayer();
	if (!LocalPlayer)
	{
		return nullptr;
	}
	
	if (URsBattleViewModel* ExistingViewModel = URsSingletonViewModelBase::GetViewModel<URsBattleViewModel>(LocalPlayer, false))
	{
		return ExistingViewModel;
	}

	URsBattleSubsystem* BattleSubsystem = LocalPlayer->GetSubsystem<URsBattleSubsystem>();
	if (!BattleSubsystem)
	{
		return nullptr;
	}
	
	return URsSingletonViewModelBase::CreateViewModel<URsBattleViewModel>(BattleSubsystem);
}
