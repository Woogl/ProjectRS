// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Rs/UI/MVVM/RsMVVMGameSubsystem.h"
#include "RsViewModelBase.generated.h"

class URsMVVMGameSubsystem;

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;

	template <typename T>
	static T* CreateViewModel(UObject* Model);
	
	template <typename T>
	static T* CreateViewModel(const UObject* Model);

	template <typename T>
	static T* CreateSingletonViewModel(UObject* Model, bool bWarnIfNotFound = true);

	template <typename T>
	static T* GetSingletonViewModel(const UObject* WorldContext, bool bWarnIfNotFound = true);
	
	template <typename T>
	T* GetModel() const;
	
protected:
	virtual void Initialize() {};
	virtual void Deinitialize() {};

	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> Model;
};

template <typename T>
T* URsViewModelBase::CreateViewModel(UObject* Model)
{
	static_assert(TIsDerivedFrom<T, URsViewModelBase>::IsDerived, "T must be derived from URsViewModelBase");
	if (!ensure(Model))
	{
		return nullptr;
	}
	T* ViewModel = NewObject<T>(Model);
	ViewModel->Model = Model;
	static_cast<URsViewModelBase*>(ViewModel)->Initialize();
	return ViewModel;
}

template <typename T>
T* URsViewModelBase::CreateViewModel(const UObject* Model)
{
	UObject* MutableModel = const_cast<UObject*>(Model);
	return CreateViewModel<T>(MutableModel);
}

template <typename T>
T* URsViewModelBase::CreateSingletonViewModel(UObject* Model, bool bWarnIfNotFound)
{
	if (URsMVVMGameSubsystem* MVVMSubsystem = URsMVVMGameSubsystem::Get(Model))
	{
		if (T* ViewModel = CreateViewModel<T>(Model))
		{
			if (MVVMSubsystem->AddSingletonViewModel(ViewModel, bWarnIfNotFound))
			{
				return ViewModel;
			}
		}
	}
	return nullptr;
}

template <typename T>
T* URsViewModelBase::GetSingletonViewModel(const UObject* WorldContext, bool bWarnIfNotFound)
{
	if (URsMVVMGameSubsystem* MVVMSubsystem = URsMVVMGameSubsystem::Get(WorldContext))
	{
		MVVMSubsystem->GetSingletonViewModel(T::StaticClass(), bWarnIfNotFound);
	}
	return nullptr;
}

template <typename T>
T* URsViewModelBase::GetModel() const
{
	static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "T must be derived from UObject");
	return Cast<T>(Model.Get());
}
