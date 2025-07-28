// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_EffectToSelf.generated.h"

class URsEffectDefinition;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_EffectToSelf : public UAnimNotify
{
	GENERATED_BODY()

public:
	URsAnimNotify_EffectToSelf();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category="AnimNotify"))
	FGameplayTag EventTag;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
