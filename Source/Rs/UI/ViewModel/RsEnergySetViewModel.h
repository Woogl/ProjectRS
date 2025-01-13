// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "RsEnergySetViewModel.generated.h"

struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class RS_API URsEnergySetViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static URsEnergySetViewModel* CreateEnergySetViewModel(AActor* Model);

	void Initialize();

	float GetCurrentEnergy() const;
	float GetMaxEnergy() const;
	float GetEnergyRegen() const;
	
	void SetCurrentEnergy(float NewCurrentEnergy);
	void SetMaxEnergy(float NewMaxEnergy);
	void SetEnergyRegen(float NewEnergyRegen);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetEnergyPercent() const;
	
private:
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float CurrentEnergy;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float MaxEnergy;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float EnergyRegen;

	void MaxEnergyChanged(const FOnAttributeChangeData& Data);
	void CurrentEnergyChanged(const FOnAttributeChangeData& Data);
	void EnergyRegenChanged(const FOnAttributeChangeData& Data);
};
