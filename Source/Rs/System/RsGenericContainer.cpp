// Copyright 2024 Team BH.


#include "RsGenericContainer.h"

void URsGenericContainer::Reset()
{
	BoolValues.Empty();
	IntValues.Empty();
	FloatValues.Empty();
	VectorValues.Empty();
	RotatorValues.Empty();
	ObjectValues.Empty();
}

bool URsGenericContainer::GetBoolValue(FName Key) const
{
	return BoolValues.FindRef(Key);
}

int32 URsGenericContainer::GetIntValue(FName Key) const
{
	return IntValues.FindRef(Key);
}

float URsGenericContainer::GetFloatValue(FName Key) const
{
	return FloatValues.FindRef(Key);
}

FVector URsGenericContainer::GetVectorValue(FName Key) const
{
	return VectorValues.FindRef(Key);
}

FRotator URsGenericContainer::GetRotatorValue(FName Key) const
{
	return RotatorValues.FindRef(Key);
}

UObject* URsGenericContainer::GetObjectValue(FName Key) const
{
	return ObjectValues.FindRef(Key);
}

void URsGenericContainer::SetBoolValue(FName Key, bool Value)
{
	BoolValues.Add(Key, Value);
}

void URsGenericContainer::SetIntValue(FName Key, int32 Value)
{
	IntValues.Add(Key, Value);
}

void URsGenericContainer::SetFloatValue(FName Key, float Value)
{
	FloatValues.Add(Key, Value);
}

void URsGenericContainer::SetVectorValue(FName Key, FVector Value)
{
	VectorValues.Add(Key, Value);
}

void URsGenericContainer::SetRotatorValue(FName Key, FRotator Value)
{
	RotatorValues.Add(Key, Value);
}

void URsGenericContainer::SetObjectValue(FName Key, UObject* Value)
{
	ObjectValues.Add(Key, Value);
}
