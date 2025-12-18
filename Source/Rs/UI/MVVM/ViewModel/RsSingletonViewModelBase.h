// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
#include "Rs/UI/MVVM/RsMVVMGameSubsystem.h"
#include "RsSingletonViewModelBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsSingletonViewModelBase : public URsViewModelBase
{
	GENERATED_BODY()

public:
	template <typename T>
	static T* GetViewModel(const UObject* WorldContext, bool bWarnIfNotFound);

	template <typename T>
	static T* GetOrCreateViewModel(T::ModelType* Model, bool bWarnIfNotFound);
};

template <typename T>
T* URsSingletonViewModelBase::GetViewModel(const UObject* WorldContext, bool bWarnIfNotFound)
{
	if (URsMVVMGameSubsystem* MVVMSubsystem = URsMVVMGameSubsystem::Get(WorldContext))
	{
		return Cast<T>(MVVMSubsystem->GetSingletonViewModel(T::StaticClass(), bWarnIfNotFound));
	}
	return nullptr;
}

template <typename T>
T* URsSingletonViewModelBase::GetOrCreateViewModel(typename T::ModelType* Model, bool bWarnIfNotFound)
{
	if (T* ExistingViewModel = URsSingletonViewModelBase::GetViewModel<T>(Model, false))
	{
		return ExistingViewModel;
	}
	if (URsMVVMGameSubsystem* MVVMSubsystem = URsMVVMGameSubsystem::Get(Model->GetWorld()))
	{
		if (T* ViewModel = URsViewModelBase::CreateViewModel<T>(Model))
		{
			MVVMSubsystem->AddSingletonViewModel(ViewModel, bWarnIfNotFound);
			return ViewModel;
		}
	}
	return nullptr;
}
