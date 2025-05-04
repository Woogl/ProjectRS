// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "RsAbilityViewModel.generated.h"

class URsGameplayAbility;

/**
 * 
 */
UCLASS()
class RS_API URsAbilityViewModel : public UMVVMViewModelBase, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static URsAbilityViewModel* CreateRsAbilityViewModel(URsGameplayAbility* Model);

protected:
	void Initialize();

public:
	float GetCooldownDuration() const;
	float GetCooldownRemaining() const;
	int32 GetCurrentRechargeStacks() const;
	int32 GetMaxRechargeStacks() const;

	void SetCooldownDuration(float NewCooldownDuration);
	void SetCooldownRemaining(float NewCooldownRemaining);
	void SetCurrentRechargeStacks(int32 NewStacks);
	void SetMaxRechargeStacks(int32 NewStacks);

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetCooldownPercent() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	bool IsOnCooldown() const;

	UFUNCTION(BlueprintPure)
	bool IsRechargeable() const;

	// FTickableGameObject
	// NOTE: Ability class can't tick by default, but I want to display cooldown updated every frame.
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	
private:
	void HandleRechargeStacksChanged(int CurrentStacks);
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float CooldownDuration;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float CooldownRemaining;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	int32 CurrentRechargeStacks;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	int32 MaxRechargeStacks;

	UPROPERTY()
	TWeakObjectPtr<URsGameplayAbility> CachedModel;
};
