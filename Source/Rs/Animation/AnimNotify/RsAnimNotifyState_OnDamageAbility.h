// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyState_AbilityBase.h"
#include "RsAnimNotifyState_OnDamageAbility.generated.h"

class URsAbilityTask_WaitDamageEffectBlockedImmunity;
class URsAbilityTask_WaitDamageEffectApplied;
struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpecHandle;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_OnDamageAbility : public URsAnimNotifyState_AbilityBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(Categories="Effect.Damage"))
	FGameplayTagContainer ReceivedDamageTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(Categories="Ability"))
	FGameplayTag TriggeredAbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	bool bTriggerOnDamageImmunity = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	bool bEnablePerfectDodgeCapsuleCollision = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	bool bTriggerOnce = true;

private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	void HandleReceiveDamage(AActor* Source, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle);

	UFUNCTION()
	void HandleBlockDamage(FGameplayEffectSpecHandle BlockedSpec, FActiveGameplayEffectHandle ImmunityGameplayEffectHandle);
	
	TWeakObjectPtr<URsAbilityTask_WaitDamageEffectApplied> WaitAppliedTask;
	TWeakObjectPtr<URsAbilityTask_WaitDamageEffectBlockedImmunity> WaitBlockedTask;
};
