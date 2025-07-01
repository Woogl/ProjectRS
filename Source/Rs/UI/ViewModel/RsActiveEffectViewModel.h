// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsViewModelBase.h"
#include "Rs/AbilitySystem/EffectComponent/RsGameplayEffectUIDataComponent.h"
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

	URsActiveEffectViewModel();
	
	UFUNCTION(BlueprintCallable)
	static URsActiveEffectViewModel* CreateRsActiveEffectViewModel(FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION(FieldNotify, BlueprintPure)
	int32 GetStack() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetStackText() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetEffectProgress() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	UObject* GetIcon() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetDescription() const;

	void AddExtraModel(FActiveGameplayEffectHandle OtherEffectHandle);

protected:
	static const URsGameplayEffectUIDataComponent* FindRsUIData(const FActiveGameplayEffect& Effect);

	virtual void Initialize() override;
	virtual void Deinitialize() override;

	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;

	void OnEffectAdded();
	void OnEffectRenewed(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration);
	void OnEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo);
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(URsAbilityViewModel, STATGROUP_Tickables); };

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnViewModelDisabled, URsActiveEffectViewModel*);
	FOnViewModelDisabled OnViewModelDisabled;

private:
	bool IsStackable;
	
	UPROPERTY(FieldNotify, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility Visibility;

	TArray<const FActiveGameplayEffect*> ExtraModels;
	int32 Stack = 1;
	
	UPROPERTY()
	TWeakObjectPtr<URsAbilitySystemComponent> CachedASC;
	
	const FActiveGameplayEffect* CachedModel;
};
