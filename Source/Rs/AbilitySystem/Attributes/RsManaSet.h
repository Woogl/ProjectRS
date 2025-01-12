// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "RsManaSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsManaSet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	URsManaSet();
	
	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Attribute Set Overrides.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Holds the current value for Mana.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentMana)
	FGameplayAttributeData CurrentMana;
	ATTRIBUTE_ACCESSORS(URsManaSet, CurrentMana)

	// Holds the value for Maximum Mana.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(URsManaSet, MaxMana)

	// Holds the value for Mana Regeneration.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegen)
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(URsManaSet, ManaRegen)

protected:
	UFUNCTION()
	virtual void OnRep_CurrentMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_ManaRegen(const FGameplayAttributeData& OldValue);
};
