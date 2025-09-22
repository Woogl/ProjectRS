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
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Cost for ultimate skill.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentEnergy)
	FGameplayAttributeData CurrentEnergy;
	ATTRIBUTE_ACCESSORS(URsEnergySet, CurrentEnergy)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(URsEnergySet, MaxEnergy)

	// Special resource for character-specific mechanics or abilities.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentMana)
	FGameplayAttributeData CurrentMana;
	ATTRIBUTE_ACCESSORS(URsEnergySet, CurrentMana)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(URsEnergySet, MaxMana)

protected:
	UFUNCTION()
	virtual void OnRep_CurrentEnergy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CurrentMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
};
