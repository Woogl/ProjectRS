// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
#include "GameplayEffectTypes.h"
#include "RsActiveEffectListViewViewModel.generated.h"

class UAbilitySystemComponent;
class URsActiveEffectViewModel;
/**
 * 
 */
UCLASS()
class RS_API URsActiveEffectListViewViewModel : public URsViewModelBase
{
	GENERATED_BODY()

public:
	using ModelType = UAbilitySystemComponent;
	
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsActiveEffectListViewViewModel* CreateActiveEffectListViewViewModel(UAbilitySystemComponent* ASC);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	void OnEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);
	void OnEffectRemoved(URsActiveEffectViewModel* DisabledViewModel);
	
protected:
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESlateVisibility Visibility;

	UPROPERTY(FieldNotify, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<URsActiveEffectViewModel>> ActiveEffectViewModels;
};
