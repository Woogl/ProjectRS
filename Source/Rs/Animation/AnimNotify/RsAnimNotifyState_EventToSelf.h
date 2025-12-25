// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotifyState_EventToSelf.generated.h"

class URsGameplayAbility;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_EventToSelf : public URsAnimNotifyStateBase
{
	GENERATED_BODY()
	
public:
	URsAnimNotifyState_EventToSelf();
	
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
protected:
	virtual void HandleConditionSatisfied() override;
};
