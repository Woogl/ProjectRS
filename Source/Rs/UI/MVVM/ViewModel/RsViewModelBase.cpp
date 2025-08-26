// Copyright 2024 Team BH.


#include "RsViewModelBase.h"

void URsViewModelBase::BeginDestroy()
{
	Super::BeginDestroy();

	Deinitialize();
}

void URsViewModelBase::SetModel(UObject* InModel)
{
	Model = InModel;
}
