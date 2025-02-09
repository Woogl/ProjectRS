// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility.h"
#include "RsGameplayAbility_Dodge.generated.h"

class UAbilityTask_WaitGameplayEffectApplied_Self;
/**
 * 
 */
UCLASS()
class RS_API URsGameplayAbility_Dodge : public URsGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability", DisplayName = "OnPerfectDodgeSuccess")
	void K2_OnPerfectDodgeSuccess(AActor* Source, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	TObjectPtr<UAnimMontage> DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	TObjectPtr<UAnimMontage> BackstepMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	float InputTurnAroundSpeed = 750.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	float InputTurnAroundMaxDuration = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Perfect Dodge", meta = (Categories = "Event"))
	FGameplayTag PerfectDodgeStartTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Perfect Dodge", meta = (Categories = "Event"))
	FGameplayTag PerfectDodgeEndTag;
	
	// Gameplay effect that will apply to self during perfect dodge.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Perfect Dodge")
	TSubclassOf<UGameplayEffect> PerfectDodgeEffectToSelf;

	// Gameplay effect's asset tag that can dodge.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	FGameplayTagContainer DamageTagsCanDodge;

	// Gameplay effect's asset tag that cannot dodge.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Damage")
	FGameplayTagContainer DamageTagsCannotDodge;
	
	UFUNCTION()
	void PlayDashOrBackstepMontage();
	
	UFUNCTION()
	void HandleMontageCompleted();

	UFUNCTION()
	void HandleMontageCancelled();

	UFUNCTION()
	void HandlePerfectDodgeStarted(FGameplayEventData Data);
	
	UFUNCTION()
	void HandlePerfectDodgeEnded(FGameplayEventData Data);

	UFUNCTION()
	void HandleDamageEffectApplied(AActor* Source, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle);

private:
	UPROPERTY(Transient)
	TObjectPtr<UAnimMontage> MontageToPlay;
	
	UPROPERTY(Transient)
	TObjectPtr<UAbilityTask_WaitGameplayEffectApplied_Self> WaitDamageEffectTask;

	UPROPERTY(Transient)
	FActiveGameplayEffectHandle PerfectDodgeSelfEffectHandle;
};
