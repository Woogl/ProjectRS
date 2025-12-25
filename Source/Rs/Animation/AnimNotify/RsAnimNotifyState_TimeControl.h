// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotifyState_TimeControl.generated.h"

enum class ERsTimeControlPriority : uint8;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_TimeControl : public URsAnimNotifyStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float TimeDilation = 0.f;

	UPROPERTY(EditAnywhere)
	float BlendTime = 0.f;
	
	UPROPERTY(EditAnywhere)
	ERsTimeControlPriority Priority;

protected:
	virtual void HandleConditionSatisfied() override;
};
