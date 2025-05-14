// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyState_Targeting.h"
#include "Rs/AbilitySystem/Effect/RsBuffEffect.h"
#include "RsAnimNotifyState_BuffToTarget.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_BuffToTarget : public URsAnimNotifyState_Targeting
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	FRsBuffCoefficient BuffCoefficient;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
};
