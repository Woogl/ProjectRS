// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyState_Targeting.h"
#include "RsAnimNotifyState_EffectToTarget.generated.h"

class URsEffectDefinition;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_EffectToTarget : public URsAnimNotifyState_Targeting
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	URsEffectDefinition* EffectDefinition;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

};
