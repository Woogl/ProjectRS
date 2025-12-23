// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Rs/AbilitySystem/Attributes/RsStatQuery.h"
#include "UObject/Object.h"
#include "RsCondition.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, Abstract)
class RS_API URsConditionBase : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual bool IsSatisfied(UObject* ContextObject) const { return false; }
};


UCLASS()
class RS_API URsCondition_StatQuery : public URsConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FRsStatQuery Query;
	
	virtual bool IsSatisfied(UObject* ContextObject) const override;
};
