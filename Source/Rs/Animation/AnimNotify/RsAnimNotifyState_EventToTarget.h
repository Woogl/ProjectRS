// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyState_TargetingBase.h"
#include "RsAnimNotifyState_EventToTarget.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_EventToTarget : public URsAnimNotifyState_TargetingBase
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_EventToTarget();
	virtual FString GetNotifyName_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	TWeakObjectPtr<UGameplayAbility> CurrentAbility;
};
