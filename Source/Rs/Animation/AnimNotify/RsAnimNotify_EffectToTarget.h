// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotify_Targeting.h"
#include "RsAnimNotify_EffectToTarget.generated.h"

class URsEffectDefinition;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_EffectToTarget : public URsAnimNotify_Targeting
{
	GENERATED_BODY()

public:
	URsAnimNotify_EffectToTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category="AnimNotify"))
	FGameplayTag EventTag;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
