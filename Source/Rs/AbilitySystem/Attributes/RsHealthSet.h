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
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(URsHealthSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URsHealthSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Barrier)
	FGameplayAttributeData Barrier;
	ATTRIBUTE_ACCESSORS(URsHealthSet, Barrier)

	// Used in the RsHealthDamageCalculation to calculate final health damage.
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData BaseDamage;
	ATTRIBUTE_ACCESSORS(URsHealthSet, BaseDamage)
	
	// Applied to reduce the "CurrentHealth" value. (aka. True damage)
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData FinalDamage;
	ATTRIBUTE_ACCESSORS(URsHealthSet, FinalDamage)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Healing;
	ATTRIBUTE_ACCESSORS(URsHealthSet, Healing)
	
	mutable FRsAttributeEvent OnCurrentHealthChanged;
	mutable FRsAttributeEvent OnMaxHealthChanged;
	mutable FRsAttributeEvent OnOutOfHealth;

protected:
	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Barrier(const FGameplayAttributeData& OldValue);

	// Gameplay cue
	FGameplayTag HealthDamageCueTag;
	FGameplayTag HealingCueTag;
	
private:
	// Used to track when the health reaches 0.
	bool bOutOfHealth = false;
	
	// Store the health before any changes 
	float MaxHealthBeforeChange = 0.f;
	float CurrentHealthBeforeChange = 0.f;
};
