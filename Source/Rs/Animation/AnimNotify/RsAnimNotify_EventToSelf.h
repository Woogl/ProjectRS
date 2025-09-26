// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_EventToSelf.generated.h"

class URsEffectDefinition;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_EventToSelf : public UAnimNotify
{
	GENERATED_BODY()

public:
	URsAnimNotify_EventToSelf();
	virtual FString GetNotifyName_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
