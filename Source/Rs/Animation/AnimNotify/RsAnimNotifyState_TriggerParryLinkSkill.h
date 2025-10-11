// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyState_TargetingBase.h"
#include "RsAnimNotifyState_TriggerParryLinkSkill.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_TriggerParryLinkSkill : public URsAnimNotifyState_TargetingBase
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_TriggerParryLinkSkill();
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
