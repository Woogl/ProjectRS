// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotify_Targeting.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"
#include "RsAnimNotify_ApplyEffectToTarget.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_ApplyEffectToTarget : public URsAnimNotify_Targeting
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	FRsEffectCoefficient EffectCoefficient;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
