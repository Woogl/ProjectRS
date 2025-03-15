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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	TObjectPtr<UAnimMontage> DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	TObjectPtr<UAnimMontage> BackstepMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	float TurnAroundSpeed = 800.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Montage")
	float TurnAroundMaxDuration = 0.15f;
	
	UFUNCTION()
	void PlayDashOrBackstepMontage();
	
	UFUNCTION()
	void HandleMontageCompleted();

	UFUNCTION()
	void HandleMontageCancelled();
	
private:
	UPROPERTY(Transient)
	TObjectPtr<UAnimMontage> MontageToPlay;
};
