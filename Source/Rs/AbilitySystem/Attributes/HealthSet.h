// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "HealthSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API UHealthSet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	UHealthSet();

	// Attribute Set Overrides.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Used to create a local copy of Damage which is then subtracted from Current Health.
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UHealthSet, Damage)

	// Used to create a local copy of Healing which is then added to Current Health.
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos))
	FGameplayAttributeData Healing;
	ATTRIBUTE_ACCESSORS(UHealthSet, Healing)
	
	// Holds the current value for Health.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UHealthSet, CurrentHealth)

	// Holds the value for Maximum Health.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHealthSet, MaxHealth)

	// Holds the value for Health Regeneration.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UHealthSet, HealthRegen)

protected:
	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_HealthRegen(const FGameplayAttributeData& OldValue);
};
