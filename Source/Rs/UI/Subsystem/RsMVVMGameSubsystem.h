// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "MVVMGameSubsystem.h"
#include "Rs/RsLogChannels.h"
#include "RsMVVMGameSubsystem.generated.h"

class URsViewModelBase;
/**
 * 
 */
UCLASS()
class RS_API URsMVVMGameSubsystem : public UMVVMGameSubsystem
{
	GENERATED_BODY()

public:
	static URsMVVMGameSubsystem* Get(UObject* WorldContext);
	
	template <class T>
	static T* GetOrCreateSingletonViewModel(UObject* Model);
};

template <typename T>
T* URsMVVMGameSubsystem::GetOrCreateSingletonViewModel(UObject* Model)
{
	static_assert(TIsDerivedFrom<T, URsViewModelBase>::Value, "T must derive from URsViewModelBase");
	if (!Model)
	{
		return nullptr;
	}
	URsMVVMGameSubsystem* Subsystem = URsMVVMGameSubsystem::Get(Model);
	if (!Subsystem)
	{
		return nullptr;
	}
	UMVVMViewModelCollectionObject* ViewModelCollection = Subsystem->GetViewModelCollection();
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
		FMVVMViewModelContext Context(CreatedViewModel->GetClass(), CreatedViewModel->GetFName());
		Subsystem->GetViewModelCollection()->AddViewModelInstance(Context, CreatedViewModel);	
			
		return CreatedViewModel;
	}
	
	UE_LOG(RsLog, Warning, TEXT("Failed to GetSingletonViewModel(): %s"), *GetNameSafe(T::StaticClass()));
	return nullptr;
}