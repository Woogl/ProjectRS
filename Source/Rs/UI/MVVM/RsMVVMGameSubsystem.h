// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "MVVMGameSubsystem.h"
#include "Rs/RsLogChannels.h"
#include "RsMVVMGameSubsystem.generated.h"

class UCommonLocalPlayer;
class URsViewModelBase;
/**
 * 
 */
UCLASS()
class RS_API URsMVVMGameSubsystem : public UMVVMGameSubsystem
{
	GENERATED_BODY()

public:
	static URsMVVMGameSubsystem* Get(const UObject* WorldContext);
	
	void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	
	template <class T>
	static T* CreateSingletonViewModel(UObject* Model, bool bWarnIfNotFound = true);

	template <class T>
	static T* GetSingletonViewModel(const UObject* WorldContext, bool bWarnIfNotFound = true);

private:
	void CreateSingletonViewModels_PlayerController(UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController);
	FDelegateHandle PlayerAddedDelegateHandle;
};

template <typename T>
T* URsMVVMGameSubsystem::CreateSingletonViewModel(UObject* Model, bool bWarnIfNotFound)
{
	static_assert(TIsDerivedFrom<T, URsViewModelBase>::Value, "T must derive from URsViewModelBase");
	
	URsMVVMGameSubsystem* RsMVVMSubsystem = URsMVVMGameSubsystem::Get(Model);
	if (!RsMVVMSubsystem)
	{
		return nullptr;
	}
	UMVVMViewModelCollectionObject* ViewModelCollection = RsMVVMSubsystem->GetViewModelCollection();
	if (!ViewModelCollection)
	{
		return nullptr;
	}

	// Get registered view model if available.
	if (UMVVMViewModelBase* RegisteredViewModel = ViewModelCollection->FindFirstViewModelInstanceOfType(T::StaticClass()))
	{
		return Cast<T>(RegisteredViewModel);
	}

	// Crete new view model and register.
	if (T* CreatedViewModel = NewObject<T>(Model))
	{
		CreatedViewModel->Initialize();
		FMVVMViewModelContext Context(CreatedViewModel->GetClass(), CreatedViewModel->GetClass()->GetFName());
		RsMVVMSubsystem->GetViewModelCollection()->AddViewModelInstance(Context, CreatedViewModel);	
		return CreatedViewModel;
	}

	if (bWarnIfNotFound)
	{
		UE_LOG(RsLog, Warning, TEXT("Failed to CreateSingletonViewModel(): %s"), *GetNameSafe(T::StaticClass()));
	}
	return nullptr;
}

template <class T>
T* URsMVVMGameSubsystem::GetSingletonViewModel(const UObject* WorldContext, bool bWarnIfNotFound)
{
	static_assert(TIsDerivedFrom<T, URsViewModelBase>::Value, "T must derive from URsViewModelBase");
	
	URsMVVMGameSubsystem* RsMVVMSubsystem = URsMVVMGameSubsystem::Get(WorldContext);
	if (!RsMVVMSubsystem)
	{
		return nullptr;
	}
	UMVVMViewModelCollectionObject* ViewModelCollection = RsMVVMSubsystem->GetViewModelCollection();
	if (!ViewModelCollection)
	{
		return nullptr;
	}

	// Get registered view model if available.
	if (UMVVMViewModelBase* RegisteredViewModel = ViewModelCollection->FindFirstViewModelInstanceOfType(T::StaticClass()))
	{
		return Cast<T>(RegisteredViewModel);
	}

	if (bWarnIfNotFound)
	{
		UE_LOG(RsLog, Warning, TEXT("Failed to GetSingletonViewModel(): %s"), *GetNameSafe(T::StaticClass()));
	}
	return nullptr;
}
