// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsViewModelBase.h"
#include "Components/SlateWrapperTypes.h"
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
	URsActiveEffectViewModel();
	
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsActiveEffectViewModel* CreateRsActiveEffectViewModel(FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION(FieldNotify, BlueprintPure)
	ESlateVisibility GetVisibility() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	int32 GetStack() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetStackText() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	float GetEffectProgress() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	int32 GetPriority() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	UObject* GetIcon() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetDescription() const;

protected:
	static const URsUIDataEffectComponent* FindRsUIData(const FActiveGameplayEffect& Effect)
	{
		if (TObjectPtr<const UGameplayEffect> EffectDef = Effect.Spec.Def)
		{
			if (const URsUIDataEffectComponent* UIData = EffectDef->FindComponent<URsUIDataEffectComponent>())
			{
				return UIData;
			}
		}
		return nullptr;
	};

	virtual void Initialize() override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;

	void OnEffectAdded();
	void OnEffectRenewed(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration);
	void OnEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo);
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(URsAbilityViewModel, STATGROUP_Tickables); };

public:
	DECLARE_DELEGATE_OneParam(FOnViewModelDisabled, URsActiveEffectViewModel*);
	FOnViewModelDisabled OnViewModelDisabled;

private:
	const FActiveGameplayEffect* GetActiveEffect() const;

	bool IsStackable = false;
	
	TWeakObjectPtr<const URsUIDataEffectComponent> CachedUIData;
	TWeakObjectPtr<URsAbilitySystemComponent> CachedASC;
	FActiveGameplayEffectHandle CachedEffectHandle;
};
