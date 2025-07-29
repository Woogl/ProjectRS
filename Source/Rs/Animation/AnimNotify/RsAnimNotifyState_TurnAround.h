// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_TurnAround.generated.h"

class ARsCharacterBase;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_TurnAround : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_TurnAround();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTurnAroundSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingCollision Collision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingFilter Filter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Targeting")
	FRsTargetingSorter Sorter;
	
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	TWeakObjectPtr<AActor> TurnTarget;
	bool bShouldTurn = true;
};
