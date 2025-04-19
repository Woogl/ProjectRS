// Copyright 2024 Team BH.


#include "RsGenericContainer.h"

void URsGenericContainer::Reset()
{
	Values.Empty();
}

bool URsGenericContainer::GetValueAsBool(FName Key) const
{
	return GetValue<bool>(Key);
}

void URsGenericContainer::SetValueAsBool(FName Key, bool Value)
{
	SetValue<bool>(Key, Value);
}
