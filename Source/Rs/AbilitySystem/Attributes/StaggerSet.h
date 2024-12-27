// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSet.h"
#include "StaggerSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API UStaggerSet : public URsAttributeSet
{
	GENERATED_BODY()

public:
	UStaggerSet();

	// Attribute Set Overrides.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Holds the current value for Stamina.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentStagger)
	FGameplayAttributeData CurrentStagger;
	ATTRIBUTE_ACCESSORS(UStaggerSet, CurrentStagger)

	// Holds the value for Maximum Stamina.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStagger)
	FGameplayAttributeData MaxStagger;
	ATTRIBUTE_ACCESSORS(UStaggerSet, MaxStagger)

	// Holds the current value for Stamina Regeneration.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaggerRegen)
	FGameplayAttributeData StaggerRegen;
	ATTRIBUTE_ACCESSORS(UStaggerSet, StaggerRegen)

protected:
	UFUNCTION()
	virtual void OnRep_CurrentStagger(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxStagger(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_StaggerRegen(const FGameplayAttributeData& OldValue);
};
