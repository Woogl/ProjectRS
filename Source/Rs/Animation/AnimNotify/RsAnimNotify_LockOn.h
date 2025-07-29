// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotify_LockOn.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotify_LockOn : public UAnimNotify
{
	GENERATED_BODY()

public:
	URsAnimNotify_LockOn();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingCollision Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingFilter Filter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingSorter Sorter;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
