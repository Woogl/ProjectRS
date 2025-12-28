// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyBase.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_MoveTo.generated.h"

class ARsCharacterBase;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_MoveTo : public URsAnimNotifyStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float AcceptableRadius = 150.f;

	UPROPERTY(EditAnywhere)
	FRsTargetingParams TargetingParams;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
private:
	struct FMoveToRuntimeData
	{
		TWeakObjectPtr<AActor> TurnTarget;
		float AcceptableRadius = 0.f;
		bool bShouldMove = false;
	};
	
	TMap<TWeakObjectPtr<USkeletalMeshComponent>, FMoveToRuntimeData> RuntimeDataMap;
};