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
	
protected:
	virtual void Initialize() {};
	virtual void Deinitialize() {};

	// TODO: Refactoring
	template<typename T>
	T* GetModel() const
	{
		return GetTypedOuter<T>();
	}
	
	// TWeakObjectPtr<UObject> Model;
	//
	// template<typename T>
	// T* GetModel() const
	// {
	// 	static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "T must be derived from UObject");
	// 	return Cast<T>(Model.Get());
	// }
	//
	// template<typename T>
	// void SetModel(T* InModel)
	// {
	// 	static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "T must be derived from UObject");
	// 	Model = InModel;
	// }
};
