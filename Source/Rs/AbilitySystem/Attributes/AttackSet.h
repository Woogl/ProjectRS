// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "AttackSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API UAttackSet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	UAttackSet();

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Value related to Damage dealt to enemies.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack)
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UAttackSet, Attack)

	// Value related to Stagger gain to enemies
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Impact)
	FGameplayAttributeData Impact;
	ATTRIBUTE_ACCESSORS(UAttackSet, Impact)

protected:
	UFUNCTION()
	virtual void OnRep_Attack(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Impact(const FGameplayAttributeData& OldValue);
};
