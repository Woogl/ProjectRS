// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Rs/RsLogChannels.h"
#include "RsGenericContainer.generated.h"

using FGenericTypes = TVariant<bool, int32, float, FVector, FRotator, UObject*>;

/**
 * 
 */
UCLASS(Blueprintable)
class RS_API URsGenericContainer : public UObject
{
	GENERATED_BODY()

private:
	TMap<FName, FGenericTypes> Values;

public:
	UFUNCTION(BlueprintCallable)
	void Reset();

	UFUNCTION(BlueprintCallable)
	bool GetValueAsBool(FName Key) const;

	UFUNCTION(BlueprintCallable)
	void SetValueAsBool(FName Key, bool Value);

	template<typename T>
	void SetValue(FName Key, const T& Value)
	{
		Values.Add(Key, FGenericTypes(TInPlaceType<T>(), Value));
	}

	template<typename T>
	T GetValue(FName Key) const
	{
		const FGenericTypes* Found = Values.Find(Key);
		if (Found != nullptr)
		{
			if (const T* Result = Found->TryGet<T>())
			{
				return *Result;
			}
		}
		else
		{
			UE_LOG(RsLog, Warning, TEXT("Cannot find for key '%s' in '%s'."), *Key.ToString(), *this->GetName());
		}
		return T{};
	}
};
