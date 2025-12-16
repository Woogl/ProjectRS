// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotify_TargetingBase.h"
#include "RsAnimNotify_EventToTarget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_EventToTarget : public URsAnimNotify_TargetingBase
{
	GENERATED_BODY()

public:
	URsAnimNotify_EventToTarget();
	virtual FString GetNotifyName_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
