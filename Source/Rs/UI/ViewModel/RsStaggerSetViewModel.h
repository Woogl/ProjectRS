// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
#include "RsStaggerSetViewModel.generated.h"

struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS()
class RS_API URsStaggerSetViewModel : public URsViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static URsStaggerSetViewModel* CreateStaggerSetViewModel(AActor* Model);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

public:
	float GetCurrentStagger() const;
	float GetMaxStagger() const;
	float GetStaggerRegen() const;
	
	void SetCurrentStagger(float NewCurrentStagger);
	void SetMaxStagger(float NewMaxStagger);
	void SetStaggerRegen(float NewStaggerRegen);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetStaggerPercent() const;
	
private:
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float CurrentStagger;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float MaxStagger;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	float StaggerRegen;

	void MaxStaggerChanged(const FOnAttributeChangeData& Data);
	void CurrentStaggerChanged(const FOnAttributeChangeData& Data);
	void StaggerRegenChanged(const FOnAttributeChangeData& Data);
};
