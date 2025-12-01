// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCharacterBase.h"
#include "RsEnemyCharacter.generated.h"

class URsSpeedSet;
class URsDefenseSet;
class URsAttackSet;
class URsEnergySet;
class URsStaggerSet;
class URsHealthSet;
/**
 * Base class to use for Server controlled Characters.
 * This class contains its own Ability System Component.
 */
UCLASS(Abstract, Blueprintable)
class RS_API ARsEnemyCharacter : public ARsCharacterBase
{
	GENERATED_BODY()
	
public:
	ARsEnemyCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	
	virtual void InitAbilitySystem() override;
	
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
};
