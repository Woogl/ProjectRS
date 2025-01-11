// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "RsDefenseSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsDefenseSet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	URsDefenseSet();

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Value related to damage reduction when attacked
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(URsDefenseSet, Defense)

protected:
	UFUNCTION()
	virtual void OnRep_Defense(const FGameplayAttributeData& OldValue);
};
