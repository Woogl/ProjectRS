// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Rs/AbilitySystem/Attributes/RsStatQuery.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "UObject/Object.h"
#include "RsCondition.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
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
	FRsStatQuery StatQuery;
	
	virtual bool IsSatisfied(UObject* ContextObject) const override;
};

UCLASS()
class RS_API URsCondition_TagRequirements : public URsConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagRequirements TagRequirements;
	
	virtual bool IsSatisfied(UObject* ContextObject) const override;
};

UCLASS()
class RS_API URsCondition_Targeting : public URsConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	FRsTargetingParams TargetingParams;
	
	virtual bool IsSatisfied(UObject* ContextObject) const override;
};

UCLASS()
class RS_API URsCondition_And : public URsConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Instanced, EditDefaultsOnly, BlueprintReadWrite)
	TArray<URsConditionBase*> Conditions;
	
	virtual bool IsSatisfied(UObject* ContextObject) const override;
};

UCLASS()
class RS_API URsCondition_Or : public URsConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Instanced, EditDefaultsOnly, BlueprintReadWrite)
	TArray<URsConditionBase*> Conditions;
	
	virtual bool IsSatisfied(UObject* ContextObject) const override;
};