// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Rs/AbilitySystem/Effect/RsBuffEffect.h"
#include "RsAnimNotify_BuffToSelf.generated.h"

struct FRsBuffCoefficient;
class URsBuffEffect;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_BuffToSelf : public UAnimNotify
{
	GENERATED_BODY()

public:
	URsAnimNotify_BuffToSelf();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	FRsBuffCoefficient BuffCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	float Duration;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
