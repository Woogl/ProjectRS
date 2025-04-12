// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotifyState_AbilityBase.h"
#include "RsAnimNotifyState_HitStop.generated.h"

struct FGameplayEffectSpecHandle;
class URsAbilityTask_PauseMontage;
class URsAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_HitStop : public URsAnimNotifyState_AbilityBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	float Duration = 0.25f;

private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	void HandleDealDamage(UAbilitySystemComponent* TargetASC, FGameplayEffectSpecHandle DamageEffectHandle);

	TWeakObjectPtr<URsAbilityTask_PauseMontage> PauseMontageTask;
};
