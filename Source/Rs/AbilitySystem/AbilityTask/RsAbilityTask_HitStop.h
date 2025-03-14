// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RsAbilityTask_HitStop.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitStopFinished);

/**
 * 
 */
UCLASS()
class RS_API URsAbilityTask_HitStop : public UAbilityTask
{
	GENERATED_BODY()

public:
	URsAbilityTask_HitStop();

	UPROPERTY(BlueprintAssignable)
	FOnHitStopFinished OnFinished;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URsAbilityTask_HitStop* StartHitStop(UGameplayAbility* OwningAbility, float Duration = 0.2f);

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityIsEnding) override;
	
	void HandleEndTimer();
	
private:
	FTimerHandle Timer;
	float Duration;
};
