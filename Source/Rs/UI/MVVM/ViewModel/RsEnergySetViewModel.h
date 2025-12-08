// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAttributeSetViewModelBase.h"
#include "RsEnergySetViewModel.generated.h"

class URsEnergySet;
struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class RS_API URsEnergySetViewModel : public URsAttributeSetViewModelBase
{
	GENERATED_BODY()

public:
	using ModelType = URsEnergySet;
	
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsEnergySetViewModel* CreateEnergySetViewModel(const URsEnergySet* EnergySet);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

public:
	float GetCurrentUltimate() const;
	void SetCurrentUltimate(float Value);
	
	float GetMaxUltimate() const;
	void SetMaxUltimate(float Value);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetUltimatePercent() const;
	
	float GetCurrentMana() const;
	void SetCurrentMana(float Value);
	
	float GetMaxMana() const;
	void SetMaxMana(float Value);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetManaPercent() const;
	
private:
	void MaxUltimateChanged(const FOnAttributeChangeData& Data);
	void CurrentUltimateChanged(const FOnAttributeChangeData& Data);
	void MaxManaChanged(const FOnAttributeChangeData& Data);
	void CurrentManaChanged(const FOnAttributeChangeData& Data);
	
	UPROPERTY(FieldNotify, Getter, Setter, meta=(AllowPrivateAccess))
	float CurrentUltimate = 0.f;
	
	UPROPERTY(FieldNotify, Getter, Setter, meta=(AllowPrivateAccess))
	float MaxUltimate = 0.f;
	
	UPROPERTY(FieldNotify, Getter, Setter, meta=(AllowPrivateAccess))
	float CurrentMana = 0.f;
	
	UPROPERTY(FieldNotify, Getter, Setter, meta=(AllowPrivateAccess))
	float MaxMana = 0.f;
};
