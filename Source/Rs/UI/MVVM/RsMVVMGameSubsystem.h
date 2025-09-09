// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "MVVMGameSubsystem.h"
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
	static URsMVVMGameSubsystem* Get(const UObject* WorldContext);

	bool AddSingletonViewModel(UMVVMViewModelBase* ViewModel, bool bWarnIfNotFound = true);
	UMVVMViewModelBase* GetSingletonViewModel(TSubclassOf<URsViewModelBase> ViewModelClass, bool bWarnIfNotFound = true);
};
