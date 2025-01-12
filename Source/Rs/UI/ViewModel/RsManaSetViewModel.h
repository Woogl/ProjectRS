// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "RsManaSetViewModel.generated.h"

struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class RS_API URsManaSetViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static URsManaSetViewModel* CreateManaSetViewModel(AActor* Model);

	void Initialize();

	float GetCurrentMana() const;
	float GetMaxMana() const;
	float GetManaRegen() const;
	
	void SetCurrentMana(float NewCurrentMana);
	void SetMaxMana(float NewMaxMana);
	void SetManaRegen(float NewManaRegen);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetManaPercent() const;
	
private:
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float CurrentMana;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float MaxMana;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float ManaRegen;

	void MaxManaChanged(const FOnAttributeChangeData& Data);
	void CurrentManaChanged(const FOnAttributeChangeData& Data);
	void ManaRegenChanged(const FOnAttributeChangeData& Data);
};
