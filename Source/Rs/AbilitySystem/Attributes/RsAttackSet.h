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

	// Critical rate value. Please use 0 ~ 100 in range.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalRate)
	FGameplayAttributeData CriticalRate;
	ATTRIBUTE_ACCESSORS(URsAttackSet, CriticalRate)

	// If critical hit, deal {CriticalDamage} %. If not critical hit, deal 100 %. Please use 100 over.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalDamage)
	FGameplayAttributeData CriticalDamage;
	ATTRIBUTE_ACCESSORS(URsAttackSet, CriticalDamage)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthDamageBonus)
	FGameplayAttributeData HealthDamageBonus;
	ATTRIBUTE_ACCESSORS(URsAttackSet, HealthDamageBonus)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaggerDamageBonus)
	FGameplayAttributeData StaggerDamageBonus;
	ATTRIBUTE_ACCESSORS(URsAttackSet, StaggerDamageBonus)

protected:
	UFUNCTION()
	virtual void OnRep_Attack(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Impact(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CriticalRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CriticalDamage(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_HealthDamageBonus(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_StaggerDamageBonus(const FGameplayAttributeData& OldValue);
};
