// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueInterface.h"
#include "RsAnimNotifyState_TargetingBase.h"
#include "RsAnimNotifyState_PreHit.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_PreHit : public URsAnimNotifyState_TargetingBase
{
	GENERATED_BODY()
	
public:
	URsAnimNotifyState_PreHit();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag GrantTargetTag;
	
	// GameplayCue tag to invoke.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayCue, meta = (Categories = "GameplayCue"))
	FGameplayCueTag GameplayCue;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	TArray<AActor*> ResultActors;
};
