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

	template<typename T>
	T* GetModel() const
	{
		return GetTypedOuter<T>();
	}
};
