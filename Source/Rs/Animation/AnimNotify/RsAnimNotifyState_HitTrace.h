// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyState_Targeting.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility_Attack.h"
#include "RsAnimNotifyState_HitTrace.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_HitTrace : public URsAnimNotifyState_Targeting
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_HitTrace();

	virtual FString GetNotifyName_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	FRsDamageContext DamageContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStopTraceWhenFirstHit = false;
	
	// Maximum number of traces per tick. It will prevents gaps that could cause missed hits.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSubsteps = 8;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	// Ignore already hit actors
	UPROPERTY()
	TArray<AActor*> HitTargets;
	
	UPROPERTY()
	TOptional<FTransform> LastSocketTransform;

	bool bStopTrace = false;
};
