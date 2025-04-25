// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "RsStaggerSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsStaggerSet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	URsStaggerSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData StaggerDamage;
	ATTRIBUTE_ACCESSORS(URsStaggerSet, StaggerDamage)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentStagger)
	FGameplayAttributeData CurrentStagger;
	ATTRIBUTE_ACCESSORS(URsStaggerSet, CurrentStagger)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStagger)
	FGameplayAttributeData MaxStagger;
	ATTRIBUTE_ACCESSORS(URsStaggerSet, MaxStagger)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaggerRegen)
	FGameplayAttributeData StaggerRegen;
	ATTRIBUTE_ACCESSORS(URsStaggerSet, StaggerRegen)

protected:
	UFUNCTION()
	virtual void OnRep_CurrentStagger(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxStagger(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_StaggerRegen(const FGameplayAttributeData& OldValue);
};
