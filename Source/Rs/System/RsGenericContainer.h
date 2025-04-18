// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGenericContainer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RS_API URsGenericContainer : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TMap<FName, bool> BoolValues;

	UPROPERTY()
	TMap<FName, int32> IntValues;

	UPROPERTY()
	TMap<FName, float> FloatValues;

	UPROPERTY()
	TMap<FName, FVector> VectorValues;

	UPROPERTY()
	TMap<FName, FRotator> RotatorValues;

	UPROPERTY()
	TMap<FName, TObjectPtr<UObject>> ObjectValues;

public:
	UFUNCTION(BlueprintCallable)
	void Reset();
	
	/** Getters */
	UFUNCTION(BlueprintCallable)
	bool GetBoolValue(FName Key) const;

	UFUNCTION(BlueprintCallable)
	int32 GetIntValue(FName Key) const;

	UFUNCTION(BlueprintCallable)
	float GetFloatValue(FName Key) const;

	UFUNCTION(BlueprintCallable)
	FVector GetVectorValue(FName Key) const;

	UFUNCTION(BlueprintCallable)
	FRotator GetRotatorValue(FName Key) const;

	UFUNCTION(BlueprintCallable)
	UObject* GetObjectValue(FName Key) const;

	/** Setters */
	UFUNCTION(BlueprintCallable)
	void SetBoolValue(FName Key, bool Value);
	
	UFUNCTION(BlueprintCallable)
	void SetIntValue(FName Key, int32 Value);

	UFUNCTION(BlueprintCallable)
	void SetFloatValue(FName Key, float Value);

	UFUNCTION(BlueprintCallable)
	void SetVectorValue(FName Key, FVector Value);

	UFUNCTION(BlueprintCallable)
	void SetRotatorValue(FName Key, FRotator Value);

	UFUNCTION(BlueprintCallable)
	void SetObjectValue(FName Key, UObject* Value);
};
