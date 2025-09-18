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

	// Override ModelType in child class.
	using ModelType = UObject;

	template <class T>
	static T* CreateViewModel(typename T::ModelType* Model);
	
	template <typename T>
	static T* CreateViewModel(const typename T::ModelType* Model);

	template <typename T>
	static T* CreateSingletonViewModel(typename T::ModelType* Model, bool bWarnIfNotFound = true);

	template <typename T>
	static T* GetSingletonViewModel(const UObject* WorldContext, bool bWarnIfNotFound = true);
	
	template <typename T>
	typename T::ModelType* GetModel() const;
	
protected:
	virtual void Initialize() {};
	virtual void Deinitialize() {};

	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> Model;
};

template <typename T>
T* URsViewModelBase::CreateViewModel(typename T::ModelType* Model)
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
T* URsViewModelBase::CreateViewModel(const typename T::ModelType* Model)
{
	typename T::ModelType* MutableModel = const_cast<typename T::ModelType*>(Model);
	return CreateViewModel<T>(MutableModel);
}

template <typename T>
T* URsViewModelBase::CreateSingletonViewModel(typename T::ModelType* Model, bool bWarnIfNotFound)
{
	if (URsMVVMGameSubsystem* MVVMSubsystem = URsMVVMGameSubsystem::Get(Model->GetWorld()))
	{
		if (T* ViewModel = CreateViewModel<T>(Model))
		{
			MVVMSubsystem->AddSingletonViewModel(ViewModel, bWarnIfNotFound);
			return ViewModel;
		}
	}
	return nullptr;
}

template <typename T>
T* URsViewModelBase::GetSingletonViewModel(const UObject* WorldContext, bool bWarnIfNotFound)
{
	if (URsMVVMGameSubsystem* MVVMSubsystem = URsMVVMGameSubsystem::Get(WorldContext))
	{
		return Cast<T>(MVVMSubsystem->GetSingletonViewModel(T::StaticClass(), bWarnIfNotFound));
	}
	return nullptr;
}

template <typename T>
typename T::ModelType* URsViewModelBase::GetModel() const
{
	static_assert(TIsDerivedFrom<T, URsViewModelBase>::IsDerived, "T must be derived from URsViewModelBase");
	return Cast<typename T::ModelType>(Model.Get());
}
