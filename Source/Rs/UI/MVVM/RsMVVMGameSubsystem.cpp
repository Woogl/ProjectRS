// Copyright 2025 Team BH.


#include "RsMVVMGameSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Rs/RsLogChannels.h"
#include "ViewModel/RsViewModelBase.h"

URsMVVMGameSubsystem* URsMVVMGameSubsystem::Get(const UObject* WorldContext)
{
	if (!WorldContext)
	{
		return nullptr;
	}
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContext))
	{
		return GameInstance->GetSubsystem<URsMVVMGameSubsystem>();
	}
	return nullptr;
}

bool URsMVVMGameSubsystem::AddSingletonViewModel(UMVVMViewModelBase* ViewModel, bool bWarnIfNotFound)
{
	if (!ViewModel)
	{
		return false;
	}
	FName ViewModelClassName = ViewModel->GetClass()->GetFName();
	FMVVMViewModelContext Context(ViewModel->GetClass(), ViewModelClassName);
	bool bSuccess = GetViewModelCollection()->AddViewModelInstance(Context, ViewModel);
	if (!bSuccess && bWarnIfNotFound)
	{
		UE_LOG(RsUILog, Warning, TEXT("Failed to AddSingletonViewModel(): %s"), *ViewModelClassName.ToString());
	}
	return bSuccess;
}

UMVVMViewModelBase* URsMVVMGameSubsystem::GetSingletonViewModel(TSubclassOf<URsViewModelBase> ViewModelClass, bool bWarnIfNotFound)
{
	if (UMVVMViewModelBase* RegisteredViewModel = GetViewModelCollection()->FindFirstViewModelInstanceOfType(ViewModelClass))
	{
		return RegisteredViewModel;
	}
	if (bWarnIfNotFound)
	{
		UE_LOG(RsUILog, Warning, TEXT("Failed to GetSingletonViewModel(): %s"), *GetNameSafe(ViewModelClass));
	}
	return nullptr;
}
