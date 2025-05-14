// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Rs/AbilitySystem/Effect/RsBuffEffect.h"
#include "RsAnimNotifyState_BuffToSelf.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_BuffToSelf : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_BuffToSelf();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	FRsBuffCoefficient BuffCoefficient;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
};
