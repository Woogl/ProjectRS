// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RsAbilityTask_TurnToLocation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnToLocationFinished);

/**
 * 
 */
UCLASS()
class RS_API URsAbilityTask_TurnToLocation : public UAbilityTask
{
	GENERATED_BODY()

public:
	URsAbilityTask_TurnToLocation();

	UPROPERTY(BlueprintAssignable)
	FOnTurnToLocationFinished OnFinish;

	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URsAbilityTask_TurnToLocation* TurnToLocation(UGameplayAbility* OwningAbility, FVector TargetLocation, float RotatingSpeed = 300.f, float MaxDuration = 1.f, bool bRotatePitch = false, bool bRotateYaw = true, bool bRotateRoll = false);

protected:
	virtual void Activate() override;
	
	/** Tick function for this task, if bTickingTask == true */
	virtual void TickTask(float DeltaTime) override;

	virtual void PreDestroyFromReplication() override;

private:
	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY(Replicated)
	float RotatingSpeed;

	UPROPERTY(Replicated)
	float MaxDuration;

	bool bRotatePitch;
	bool bRotateYaw;
	bool bRotateRoll;
	
	float StartTime;
	float EndTime;

	bool bIsFinished;
};
