// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "DefenseSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API UDefenseSet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	UDefenseSet();

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Value related to damage reduction when attacked
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UDefenseSet, Defense)

	// Value related to stagger gain reduction when attacked
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Will)
	FGameplayAttributeData Will;
	ATTRIBUTE_ACCESSORS(UDefenseSet, Will)

protected:
	UFUNCTION()
	virtual void OnRep_Defense(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Will(const FGameplayAttributeData& OldValue);
};
