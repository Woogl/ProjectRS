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
	
	template<typename T>
	static T* CreateViewModel(UObject* Model);
	
	template<typename T>
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
T* URsViewModelBase::GetModel() const
{
	static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "T must be derived from UObject");
	return Cast<T>(Model.Get());
}
