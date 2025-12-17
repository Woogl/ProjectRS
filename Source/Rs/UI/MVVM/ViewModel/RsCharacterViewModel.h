// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "RsViewModelBase.h"
#include "RsCharacterViewModel.generated.h"

class URsEnergySetViewModel;
class URsActiveEffectViewModel;
class URsStaggerSetViewModel;
class URsHealthSetViewModel;
class UAbilitySystemComponent;
class ARsCharacterBase;
/**
 * 
 */
UCLASS()
class RS_API URsCharacterViewModel : public URsViewModelBase
{
	GENERATED_BODY()

public:
	using ModelType = ARsCharacterBase;
	
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsCharacterViewModel* CreateRsCharacterViewModel(ARsCharacterBase* Character);
	
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	bool TryActivateAbility(FGameplayTag AbilityTag);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	void OnEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);
	void OnEffectRemoved(URsActiveEffectViewModel* DisabledViewModel);

public:
	UFUNCTION(BlueprintPure)
	UObject* GetPortrait() const;

	UFUNCTION(BlueprintPure)
	FText GetCharacterName() const;

	UFUNCTION(BlueprintPure)
	FText GetDescription() const;

	UFUNCTION(BlueprintPure)
	ESlateVisibility GetPortraitVisibility() const;
	
protected:
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsHealthSetViewModel> HealthSetViewModel;

	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsStaggerSetViewModel> StaggerSetViewModel;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsEnergySetViewModel> EnergySetViewModel;

	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	TArray<TObjectPtr<URsActiveEffectViewModel>> ActiveEffectViewModels;
};
