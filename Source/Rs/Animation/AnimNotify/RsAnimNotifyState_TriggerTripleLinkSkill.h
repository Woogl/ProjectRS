// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyState_AbilityBase.h"
#include "RsAnimNotifyState_TriggerTripleLinkSkill.generated.h"

class URsAbilityTask_WaitDamageEffectBlockedImmunity;
class URsAbilityTask_WaitDamageEffectApplied;
struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpecHandle;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_TriggerTripleLinkSkill : public URsAnimNotifyState_AbilityBase
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_TriggerTripleLinkSkill();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(Categories="Effect.Damage"))
	FGameplayTagContainer ReceivedDamageTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	bool bTriggerOnDamageImmunity = true;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UFUNCTION()
	void HandleReceiveDamage(AActor* Source, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle);

	UFUNCTION()
	void HandleBlockDamage(FGameplayEffectSpecHandle BlockedSpec, FActiveGameplayEffectHandle ImmunityGameplayEffectHandle);

	TWeakObjectPtr<URsAbilityTask_WaitDamageEffectApplied> WaitAppliedTask;
	TWeakObjectPtr<URsAbilityTask_WaitDamageEffectBlockedImmunity> WaitBlockedTask;
};
