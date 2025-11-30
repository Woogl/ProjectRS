// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetBase.h"
#include "RsSpeedSet.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsSpeedSet : public URsAttributeSetBase
{
	GENERATED_BODY()

public:
	URsSpeedSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Movement)
	FGameplayAttributeData Movement;
	ATTRIBUTE_ACCESSORS(URsSpeedSet, Movement)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ActionSpeed)
	FGameplayAttributeData ActionSpeed;
	ATTRIBUTE_ACCESSORS(URsSpeedSet, ActionSpeed)

protected:
	UFUNCTION()
	virtual void OnRep_Movement(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_ActionSpeed(const FGameplayAttributeData& OldValue);
};
