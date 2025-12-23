// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RsStatQuery.generated.h"

class UAbilitySystemComponent;

UENUM()
enum class ERsStatRelationalOperator : uint8
{
	Greater			UMETA(DisplayName = ">"),
	GreaterEqual	UMETA(DisplayName = ">="),
	EqualEqual		UMETA(DisplayName = "=="),
	NotEqual		UMETA(DisplayName = "!="),
	LessEqual		UMETA(DisplayName = "<="),
	Less			UMETA(DisplayName = "<")
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct RS_API FRsStatQuery
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta=(Categories = "Stat"))
	FGameplayTag StatTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere)
	ERsStatRelationalOperator Operator = ERsStatRelationalOperator::Greater;
	
	UPROPERTY(EditAnywhere)
	float CompareValue = 0.f;
	
	bool IsValid() const;
	bool MatchesQuery(UAbilitySystemComponent* ASC) const;
};

UCLASS(MinimalAPI)
class URsStatQueryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "RS | Stat Query Library")
	static RS_API bool MatchesStatQuery(UAbilitySystemComponent* ASC, FRsStatQuery StatQuery);
};