// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "RsPlayerState.generated.h"

class URsSpeedSet;
class URsDefenseSet;
class URsEnergySet;
class URsAttackSet;
class URsStaggerSet;
class URsHealthSet;
/**
 * This version of Player State has an Ability System Component and should be the base for your Player State subclass when using GAS.
 */
UCLASS()
class RS_API ARsPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARsPlayerState();

	// Implement the IAbilitySystemInterface. (This is used to find the Ability System Component.)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
private:
	// The Ability System Component for this PlayerState. 
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<const URsHealthSet> HealthSet;
	
	UPROPERTY()
	TObjectPtr<const URsStaggerSet> StaggerSet;
	
	UPROPERTY()
	TObjectPtr<const URsEnergySet> EnergySet;
	
	UPROPERTY()
	TObjectPtr<const URsAttackSet> AttackSet;
	
	UPROPERTY()
	TObjectPtr<const URsDefenseSet> DefenseSet;
	
	UPROPERTY()
	TObjectPtr<const URsSpeedSet> SpeedSet;

	// UPROPERTY()
	// TObjectPtr<const URsCombatSet> CombatSet;
};
