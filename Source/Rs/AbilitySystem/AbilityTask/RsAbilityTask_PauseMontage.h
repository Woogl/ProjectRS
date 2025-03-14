// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RsAbilityTask_PauseMontage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPauseMontageFinished);

/**
 * 
 */
UCLASS()
class RS_API URsAbilityTask_PauseMontage : public UAbilityTask
{
	GENERATED_BODY()

public:
	URsAbilityTask_PauseMontage();

	UPROPERTY(BlueprintAssignable)
	FOnPauseMontageFinished OnFinished;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URsAbilityTask_PauseMontage* PauseMontage(UGameplayAbility* OwningAbility, float Duration = 0.25f);

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityIsEnding) override;
	
	void HandleEndTimer();
	
private:
	FTimerHandle Timer;
	float Duration;
};
