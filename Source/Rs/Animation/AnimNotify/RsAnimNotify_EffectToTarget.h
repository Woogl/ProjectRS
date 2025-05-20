// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotify_Targeting.h"
#include "RsAnimNotify_EffectToTarget.generated.h"

class URsEffectDefinition;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_EffectToTarget : public URsAnimNotify_Targeting
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	URsEffectDefinition* EffectDefinition;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
