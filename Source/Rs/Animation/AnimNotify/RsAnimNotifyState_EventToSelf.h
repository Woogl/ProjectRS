// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyState_AbilityBase.h"
#include "RsAnimNotifyState_EventToSelf.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_EventToSelf : public URsAnimNotifyState_AbilityBase
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_EventToSelf();
	virtual FString GetNotifyName_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
