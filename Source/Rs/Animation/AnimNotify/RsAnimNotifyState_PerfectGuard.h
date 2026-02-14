// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "RsAnimNotifyState_PerfectGuard.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpecHandle;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotifyState_PerfectGuard : public URsAnimNotifyStateBase
{
	GENERATED_BODY()
	
public:
	URsAnimNotifyState_PerfectGuard();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InvincibleEffect;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> CounterEffect;
	
	UPROPERTY(EditAnywhere, meta=(Categories="Effect.Damage"))
	FGameplayTagContainer DamageTags;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UFUNCTION()
	void HandleDamageBlocked(FGameplayEffectSpecHandle BlockedSpec, FActiveGameplayEffectHandle ImmunityGameplayEffectHandle);
};
