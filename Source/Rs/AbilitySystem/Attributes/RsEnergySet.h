// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "RsEnergySet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsEnergySet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	URsEnergySet();
	
	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Attribute Set Overrides.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Holds the current value for Energy.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentEnergy)
	FGameplayAttributeData CurrentEnergy;
	ATTRIBUTE_ACCESSORS(URsEnergySet, CurrentEnergy)

	// Holds the value for Maximum Energy.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(URsEnergySet, MaxEnergy)

	// Holds the value for Energy Regeneration.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EnergyRegen)
	FGameplayAttributeData EnergyRegen;
	ATTRIBUTE_ACCESSORS(URsEnergySet, EnergyRegen)

protected:
	UFUNCTION()
	virtual void OnRep_CurrentEnergy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_EnergyRegen(const FGameplayAttributeData& OldValue);
};
