// Copyright 2024 Team BH.


#include "RsViewModelBase.h"

void URsViewModelBase::BeginDestroy()
{
	Deinitialize();
	
	Super::BeginDestroy();
}
