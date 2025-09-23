// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "RsViewModelBase.generated.h"

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
	typename T::ModelType* GetModel() const;
	
protected:
	virtual void Initialize() {};
	virtual void Deinitialize() {};

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
typename T::ModelType* URsViewModelBase::GetModel() const
{
	static_assert(TIsDerivedFrom<T, URsViewModelBase>::IsDerived, "T must be derived from URsViewModelBase");
	return Cast<typename T::ModelType>(Model.Get());
}
