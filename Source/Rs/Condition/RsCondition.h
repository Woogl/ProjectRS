// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Rs/AbilitySystem/Attributes/RsStatQuery.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "UObject/Object.h"
#include "RsCondition.generated.h"

/**
 * Pure condition check. (stateless)
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class RS_API URsConditionBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual bool IsSatisfied(UObject* ContextObject) const { return false; }
};

UCLASS()
class RS_API URsCondition_StatQuery : public URsConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FRsStatQuery StatQuery;
	
	virtual bool IsSatisfied(UObject* ContextObject) const override;
};

UCLASS()
class RS_API URsCondition_TagRequirements : public URsConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FGameplayTagRequirements TagRequirements;
	
	virtual bool IsSatisfied(UObject* ContextObject) const override;
};

UCLASS()
class RS_API URsCondition_Targeting : public URsConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FRsTargetingParams TargetingParams;
	
	virtual bool IsSatisfied(UObject* ContextObject) const override;
	
	UPROPERTY(Transient)
	mutable TArray<AActor*> OutActors;
};

UCLASS()
class RS_API URsCondition_AND : public URsConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Instanced, EditAnywhere)
	TArray<URsConditionBase*> Conditions;
	
	virtual bool IsSatisfied(UObject* ContextObject) const override;
};

UCLASS()
class RS_API URsCondition_OR : public URsConditionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Instanced, EditAnywhere)
	TArray<URsConditionBase*> Conditions;
	
	virtual bool IsSatisfied(UObject* ContextObject) const override;
};