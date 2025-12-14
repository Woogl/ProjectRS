// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RsCharacterMovementComponent.generated.h"

class UAbilitySystemComponent;
struct FGameplayTag;
class URsSpeedSet;
/**
 * 
 */
UCLASS()
class RS_API URsCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	virtual float GetMaxSpeed() const override;

protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
	void AddMovementTag(const FGameplayTag MovementTag);
	void RemoveMovementTag(const FGameplayTag MovementTag);
	
private:
	UAbilitySystemComponent* TryGetAbilitySystemComponent() const;
	
	UPROPERTY(Transient)
	mutable TObjectPtr<UAbilitySystemComponent> CachedASC;
	
	UPROPERTY(Transient)
	TObjectPtr<const URsSpeedSet> SpeedSet;
};
