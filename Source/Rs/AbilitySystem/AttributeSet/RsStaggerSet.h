// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSet.h"
#include "RsStaggerSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsStaggerSet : public URsAttributeSet
{
	GENERATED_BODY()

public:
	URsStaggerSet();

	// Attribute Set Overrides.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Holds the current value for Stamina.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentStagger)
	FGameplayAttributeData CurrentStagger;
	ATTRIBUTE_ACCESSORS(URsStaggerSet, CurrentStagger)

	// Holds the value for Maximum Stamina.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaximumStagger)
	FGameplayAttributeData MaximumStagger;
	ATTRIBUTE_ACCESSORS(URsStaggerSet, MaximumStagger)

	// Holds the current value for Stamina Regeneration.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaggerRegeneration)
	FGameplayAttributeData StaggerRegeneration;
	ATTRIBUTE_ACCESSORS(URsStaggerSet, StaggerRegeneration)

protected:
	UFUNCTION()
	virtual void OnRep_CurrentStagger(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaximumStagger(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_StaggerRegeneration(const FGameplayAttributeData& OldValue);
};
