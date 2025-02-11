// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RsAbilityTask_BulletTime.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBulletTimeFinished);

/**
 * 
 */
UCLASS()
class RS_API URsAbilityTask_BulletTime : public UAbilityTask
{
	GENERATED_BODY()

public:
	URsAbilityTask_BulletTime();
	
	UPROPERTY(BlueprintAssignable)
	FOnBulletTimeFinished OnFinished;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URsAbilityTask_BulletTime* StartBulletTime(UGameplayAbility* OwningAbility, float TimeDilation = 0.2f, float Duration = 0.2f, bool bAffectSelf = true);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URsAbilityTask_BulletTime* StartBulletTimeWithCurve(UGameplayAbility* OwningAbility, UCurveFloat* TimeDilationCurve, bool bAffectSelf = true);

protected:
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool AbilityIsEnding) override;

private:
	UPROPERTY(Replicated)
	float TimeDilation;
	
	UPROPERTY(Replicated)
	UCurveFloat* TimeDilationCurve;

	UPROPERTY(Replicated)
	bool bAffectSelf;

	float Duration;
	float ElapsedTime;
};
