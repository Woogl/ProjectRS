// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
#include "RsAbilityViewModel.generated.h"

class URsGameplayAbility;

/**
 * 
 */
UCLASS()
class RS_API URsAbilityViewModel : public URsViewModelBase, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static URsAbilityViewModel* CreateRsAbilityViewModel(URsGameplayAbility* Model);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

public:
	float GetCooldownDuration() const;
	float GetCooldownRemaining() const;
	int32 GetCurrentRechargeStacks() const;
	int32 GetMaxRechargeStacks() const;
	UObject* GetSkillIcon() const;

	void SetCooldownDuration(float NewCooldownDuration);
	void SetCooldownRemaining(float NewCooldownRemaining);
	void SetCurrentRechargeStacks(int32 NewStacks);
	void SetMaxRechargeStacks(int32 NewStacks);
	void SetSkillIcon(UObject* NewSkillIcon);

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetCooldownPercent() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	bool IsOnCooldown() const;

	UFUNCTION(BlueprintPure)
	bool IsRechargeable() const;

	UFUNCTION(BlueprintPure)
	FText GetInputKeyText() const;

	UFUNCTION(BlueprintPure)
	bool HasSkillIcon() const;

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

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	TObjectPtr<UObject> SkillIcon;

	// Pointer for Tick optimization.
	UPROPERTY()
	TWeakObjectPtr<URsGameplayAbility> CachedModel;
};
