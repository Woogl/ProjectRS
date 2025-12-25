// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotifyState_OnDamageAbility.generated.h"

class URsAbilityTask_WaitDamageEffectBlockedImmunity;
class URsAbilityTask_WaitDamageEffectApplied;
struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpecHandle;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_OnDamageAbility : public URsAnimNotifyStateBase
{
	GENERATED_BODY()

public:
	URsAnimNotifyState_OnDamageAbility();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(Categories="Effect.Damage"))
	FGameplayTagContainer ReceivedDamageTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(Categories="Ability"))
	FGameplayTag TriggeredAbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	bool bTriggerOnDamageImmunity = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	bool bTriggerOnce = true;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	void HandleReceiveDamage(AActor* Source, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle);

	UFUNCTION()
	void HandleBlockDamage(FGameplayEffectSpecHandle BlockedSpec, FActiveGameplayEffectHandle ImmunityGameplayEffectHandle);
	
	UPROPERTY(Transient)
	TObjectPtr<URsAbilityTask_WaitDamageEffectApplied> WaitAppliedTask;
	
	UPROPERTY(Transient)
	TObjectPtr<URsAbilityTask_WaitDamageEffectBlockedImmunity> WaitBlockedTask;
};
