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
	UPROPERTY()
	bool bIsCriticalHit = false;
	
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
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