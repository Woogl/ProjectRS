// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotify_EventToTarget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_EventToTarget : public URsAnimNotifyBase
{
	GENERATED_BODY()

public:
	URsAnimNotify_EventToTarget();

	UPROPERTY(EditAnywhere, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
