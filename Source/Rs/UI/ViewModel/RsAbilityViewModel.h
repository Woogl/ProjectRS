// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "RsAbilityViewModel.generated.h"

struct FActiveGameplayEffect;
class URsGameplayAbility;
/**
 * 
 */
UCLASS()
class RS_API URsAbilityViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static URsAbilityViewModel* CreateRsAbilityViewModel(URsGameplayAbility* Model);

	void Initialize();

	float GetCooldownDuration() const;
	float GetCooldownRemaining() const;

	void SetCooldownDuration(float NewCooldownDuration);
	void SetCooldownRemaining(float NewCooldownRemaining);

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetCooldownPercent() const;

private:
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float CooldownDuration;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float CooldownRemaining;

	UPROPERTY()
	FGameplayTag CooldownTag;

	UFUNCTION()
	void HandleGameplayEffectChanged(FActiveGameplayEffect& Effect);
};
