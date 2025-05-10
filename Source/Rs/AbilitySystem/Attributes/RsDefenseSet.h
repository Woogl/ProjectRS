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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// Value related to damage reduction when attacked
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(URsDefenseSet, Defense)

	// Damage immunity tier
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_InvincibleTier)
	FGameplayAttributeData InvincibleTier;
	ATTRIBUTE_ACCESSORS(URsDefenseSet, InvincibleTier)

	// Hit reaction immunity tier
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SuperArmorTier)
	FGameplayAttributeData SuperArmorTier;
	ATTRIBUTE_ACCESSORS(URsDefenseSet, SuperArmorTier)
	
protected:
	UFUNCTION()
	virtual void OnRep_Defense(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_InvincibleTier(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_SuperArmorTier(const FGameplayAttributeData& OldValue);
};
