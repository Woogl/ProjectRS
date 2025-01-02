// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "RsAttackSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAttackSet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	URsAttackSet();

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Value related to Damage dealt to enemies.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack)
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(URsAttackSet, Attack)

	// Value related to Stagger gain to enemies
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Impact)
	FGameplayAttributeData Impact;
	ATTRIBUTE_ACCESSORS(URsAttackSet, Impact)

protected:
	UFUNCTION()
	virtual void OnRep_Attack(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Impact(const FGameplayAttributeData& OldValue);
};
