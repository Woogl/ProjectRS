// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotifyState_PerfectDodge.generated.h"

class UGameplayEffect;
class URsAbilityTask_WaitDamageEffectBlockedImmunity;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_PerfectDodge : public URsAnimNotifyStateBase
{
	GENERATED_BODY()
	
public:
	URsAnimNotifyState_PerfectDodge();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InvincibleEffect;
	
	UPROPERTY(EditAnywhere, meta=(Categories="Effect.Damage"))
	FGameplayTagContainer DamageTags;
	
	UPROPERTY(EditAnywhere)
	bool bEnableJustDodgeCapsule = true;
	
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer DodgeCounterAbilityTags;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UFUNCTION()
	void HandleDamageBlocked(FGameplayEffectSpecHandle BlockedSpec, FActiveGameplayEffectHandle ImmunityGameplayEffectHandle);
	
	struct FDodgeInvincibleRuntimeData
	{
		TWeakObjectPtr<URsAbilityTask_WaitDamageEffectBlockedImmunity> DamageBlockTask;
		FActiveGameplayEffectHandle InvincibleEffectHandle;
	};
	
	TMap<TWeakObjectPtr<USkeletalMeshComponent>, FDodgeInvincibleRuntimeData> RuntimeDataMap;
};
