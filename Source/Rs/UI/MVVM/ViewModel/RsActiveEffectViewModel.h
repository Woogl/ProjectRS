// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "RsViewModelBase.h"
#include "RsActiveEffectViewModel.generated.h"

/**
 * 
 */

struct FActiveGameplayEffect;
class URsUIDataEffectComponent;

UCLASS()
class RS_API URsActiveEffectViewModel : public URsViewModelBase, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsActiveEffectViewModel* CreateRsActiveEffectViewModel(FActiveGameplayEffectHandle EffectHandle);
	
protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;
	
	void HandleEffectTimeChange(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration);
	
public:
	UFUNCTION(BlueprintPure)
	UObject* GetIcon() const;

	UFUNCTION(BlueprintPure)
	FText GetDescription() const;
	
	UFUNCTION(BlueprintPure)
	int32 GetPriority() const;
	
	float GetDuration() const;
	void SetDuration(float Value);
	
	float GetRemainingTime() const;
	void SetRemainingTime(float Value);
	
	int32 GetStacks() const;
	void SetStacks(int32 Value);
	
	int32 GetMaxStacks() const;
	void SetMaxStacks(int32 Value);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetStacksText() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetMaxStacksText() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetDurationText() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetRemainingTimeText() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetTimeProgress() const;
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	
	const FActiveGameplayEffect* GetActiveEffect() const;

private:
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float Duration;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float RemainingTime;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	int32 Stacks;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	int32 MaxStacks;
	
	const URsUIDataEffectComponent* GetUIData() const;
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
	
	FActiveGameplayEffectHandle EffectHandle;
};
