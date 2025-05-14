// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotify_Targeting.h"
#include "Rs/AbilitySystem/Effect/RsBuffEffect.h"
#include "RsAnimNotify_BuffToTarget.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_BuffToTarget : public URsAnimNotify_Targeting
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	FRsBuffCoefficient BuffCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	float Duration;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
