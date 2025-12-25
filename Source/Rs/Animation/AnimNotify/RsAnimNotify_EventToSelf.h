// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotify_EventToSelf.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_EventToSelf : public URsAnimNotifyBase
{
	GENERATED_BODY()

public:
	URsAnimNotify_EventToSelf();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
