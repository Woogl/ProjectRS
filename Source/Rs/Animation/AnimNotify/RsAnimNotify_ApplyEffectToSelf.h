// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"
#include "RsAnimNotify_ApplyEffectToSelf.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_ApplyEffectToSelf : public UAnimNotify
{
	GENERATED_BODY()

public:
	URsAnimNotify_ApplyEffectToSelf();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	FRsEffectCoefficient EffectCoefficient;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
