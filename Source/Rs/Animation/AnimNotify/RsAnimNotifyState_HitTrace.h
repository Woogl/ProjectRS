// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyState_Targeting.h"
#include "RsAnimNotifyState_HitTrace.generated.h"

class URsGameplayAbility;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_HitTrace : public URsAnimNotifyState_Targeting
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_HitTrace();

	virtual FString GetNotifyName_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	// Maximum number of traces per tick. It will prevents gaps that could cause missed hits.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSubsteps = 5;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	// Ignore already hit actors
	UPROPERTY()
	TArray<AActor*> HitTargets;
	
	UPROPERTY()
	TOptional<FTransform> LastSocketTransform;
	
	TWeakObjectPtr<URsGameplayAbility> CurrentAbility;
};
