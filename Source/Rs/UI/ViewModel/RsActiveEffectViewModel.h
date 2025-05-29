// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsViewModelBase.h"
#include "RsActiveEffectViewModel.generated.h"

class URsAbilitySystemComponent;
class URsGameplayEffectUIDataComponent;
/**
 * 
 */

UCLASS()
class RS_API URsActiveEffectViewModel : public URsViewModelBase, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static URsActiveEffectViewModel* CreateRsActiveEffectViewModel(URsAbilitySystemComponent* ASC, const FActiveGameplayEffect& Effect);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetEffectTimeRemainingPercent() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetStackDataText() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	UTexture2D* GetIcon() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetDescription() const;
	

protected:
	static const URsGameplayEffectUIDataComponent* FindRsUIData(const FActiveGameplayEffect& Effect);

	virtual void Initialize() override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnEffectAdded(const FActiveGameplayEffect& Effect);
	virtual void OnEffectRenewed(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration);
	virtual void OnEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo);

	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(URsAbilityViewModel, STATGROUP_Tickables); };
protected:
	UPROPERTY()
	TWeakObjectPtr<URsAbilitySystemComponent> CachedASC;
	
	const FActiveGameplayEffect* CachedModel;
};
