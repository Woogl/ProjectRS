// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueInterface.h"
#include "RsAnimNotifyBase.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_PreHit.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_PreHit : public URsAnimNotifyStateBase
{
	GENERATED_BODY()
	
public:
	URsAnimNotifyState_PreHit();
	
	UPROPERTY(EditAnywhere)
	FRsTargetingParams TargetingParams;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag GrantTargetTag;
	
	// GameplayCue tag to invoke.
	UPROPERTY(EditAnywhere, Category = GameplayCue, meta = (Categories = "GameplayCue"))
	FGameplayCueTag GameplayCue;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	struct FPreHitRuntimeData
	{
		TArray<TWeakObjectPtr<AActor>> Targets;
	};
	
	TMap<TWeakObjectPtr<USkeletalMeshComponent>, FPreHitRuntimeData> RuntimeDataMap;
};
