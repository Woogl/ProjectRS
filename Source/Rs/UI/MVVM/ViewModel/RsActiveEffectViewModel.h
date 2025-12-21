// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsViewModelBase.h"
#include "Rs/AbilitySystem/EffectComponent/RsUIDataEffectComponent.h"
#include "RsActiveEffectViewModel.generated.h"

class URsAbilitySystemComponent;
/**
 * 
 */

UCLASS()
class RS_API URsActiveEffectViewModel : public URsViewModelBase, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsActiveEffectViewModel* CreateRsActiveEffectViewModel(FActiveGameplayEffectHandle EffectHandle);
	
	int32 GetStacks() const;
	void SetStacks(int32 Value);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetStacksText() const;
	
	int32 GetMaxStacks() const;
	void SetMaxStacks(int32 Value);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetMaxStacksText() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	int32 GetPriority() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	UObject* GetIcon() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetDescription() const;
	
	void SetDuration(float Value);
	float GetDuration() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetDurationText() const;
	
	void SetRemainingTime(float Value);
	float GetRemainingTime() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetRemainingTimeText() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetEffectProgress() const;

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	void HandleEffectTimeChange(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration);
	void HandleEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo);
	
	UFUNCTION(BlueprintCallable)
	const URsUIDataEffectComponent* GetUIData() const;
	
public:
	DECLARE_DELEGATE_OneParam(FOnViewModelDisabled, URsActiveEffectViewModel*);
	FOnViewModelDisabled OnViewModelDisabled;
	
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

private:
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float Duration;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float RemainingTime;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	int32 Stacks;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	int32 MaxStacks;
	
	const FActiveGameplayEffect* GetActiveEffect() const;
	
	TWeakObjectPtr<const URsUIDataEffectComponent> CachedUIData;
	TWeakObjectPtr<URsAbilitySystemComponent> CachedASC;
	FActiveGameplayEffectHandle CachedEffectHandle;
};
