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
	UTexture2D* GetIcon() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetDescription() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	ERsEffectCategory GetCategory() const;

	void AddExtraModel(FActiveGameplayEffectHandle OtherEffectHandle);

protected:
	static const URsGameplayEffectUIDataComponent* FindRsUIData(const FActiveGameplayEffect& Effect);

	virtual void Initialize() override;
	virtual void Deinitialize() override;

	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;

	void OnEffectAdded(FActiveGameplayEffectHandle EffectHandle);
	void OnEffectRenewed(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration);
	void OnEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo);
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(URsAbilityViewModel, STATGROUP_Tickables); };

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnViewModelDisabled, URsActiveEffectViewModel*);
	FOnViewModelDisabled OnViewModelDisabled;

private:
	UPROPERTY(FieldNotify, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility Visibility;

	TArray<FActiveGameplayEffectHandle> ExtraModels;
	int32 Stack = 1;
	
	UPROPERTY()
	TWeakObjectPtr<URsAbilitySystemComponent> CachedASC;
	
	FActiveGameplayEffectHandle CachedModel;
};
