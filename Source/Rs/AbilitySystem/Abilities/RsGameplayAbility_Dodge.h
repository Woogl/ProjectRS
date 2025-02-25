// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsGameplayAbility.h"
#include "RsGameplayAbility_Dodge.generated.h"

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
	void K2_OnPerfectDodgeSuccess(AActor* DamageSource, FGameplayEffectSpecHandle SpecHandle, FActiveGameplayEffectHandle ActiveHandle);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	TObjectPtr<UAnimMontage> DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	TObjectPtr<UAnimMontage> BackstepMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	float TurnAroundSpeed = 750.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	float TurnAroundMaxDuration = 0.15f;

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

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability", DisplayName = "OnPerfectDodgeDetected")
	void K2_PerfectDodgeDetected(FGameplayEffectSpecHandle BlockedSpec, FActiveGameplayEffectHandle ImmunityGameplayEffectHandle);

public:
	UFUNCTION(BlueprintCallable)
	void StartPerfectDodgeDetection();
	UFUNCTION(BlueprintCallable)
	void EndPerfectDodgeDetection();
	
private:
	UPROPERTY(Transient)
	TObjectPtr<UAnimMontage> MontageToPlay;
};
