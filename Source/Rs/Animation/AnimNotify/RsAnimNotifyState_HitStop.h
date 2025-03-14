// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RsAnimNotifyState_HitStop.generated.h"

struct FGameplayEffectSpecHandle;
class UAbilitySystemComponent;
class URsAbilityTask_PauseMontage;
class URsAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_HitStop : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	float Duration = 0.25f;

private:
	virtual FString GetNotifyName_Implementation() const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	void HandleDealDamage(UAbilitySystemComponent* TargetASC, FGameplayEffectSpecHandle DamageEffectHandle);

	TWeakObjectPtr<URsAbilitySystemComponent> RsAbilitySystemComponent;
	TWeakObjectPtr<URsAbilityTask_PauseMontage> PauseMontageTask;
};
