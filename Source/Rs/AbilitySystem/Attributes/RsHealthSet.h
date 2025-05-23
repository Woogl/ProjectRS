// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "RsHealthSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsHealthSet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	URsHealthSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(URsHealthSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URsHealthSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(URsHealthSet, HealthRegen)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(URsHealthSet, Shield)

	// Applied to reduce the "CurrentHealth" value.
	// Reduces "Shield" first, then reduces "CurrentHealth" by any excess amount.
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData HealthDamage;
	ATTRIBUTE_ACCESSORS(URsHealthSet, HealthDamage)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Healing;
	ATTRIBUTE_ACCESSORS(URsHealthSet, Healing)

	// Used in the RsHealthDamageCalculation to calculate final health damage.
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BaseDamage;
	ATTRIBUTE_ACCESSORS(URsHealthSet, BaseDamage)

protected:
	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_HealthRegen(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldValue);
};
