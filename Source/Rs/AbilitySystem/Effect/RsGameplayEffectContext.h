// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "RsGameplayEffectContext.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool bIsCriticalHit = false;
	int16 EffectTableIndex = INDEX_NONE;
	FName EffectRowName = NAME_None;
	
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual FGameplayEffectContext* Duplicate() const override;
};

template<>
struct TStructOpsTypeTraits<FRsGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FRsGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};