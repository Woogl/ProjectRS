// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
#include "RsCharacterViewModel.generated.h"

class URsStaggerSetViewModel;
class URsHealthSetViewModel;
class ARsCharacterBase;
/**
 * 
 */
UCLASS()
class RS_API URsCharacterViewModel : public URsViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static URsCharacterViewModel* CreateRsCharacterViewModel(ARsCharacterBase* Character);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	TWeakObjectPtr<ARsCharacterBase> CachedModel;

public:
	UFUNCTION(BlueprintPure)
	FText GetCharacterName() const;

	UFUNCTION(BlueprintPure)
	UObject* GetPortrait() const;

	UFUNCTION(BlueprintPure)
	ESlateVisibility GetPortraitVisibility() const;
	
protected:
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsHealthSetViewModel> HealthSetViewModel;

	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsStaggerSetViewModel> StaggerSetViewModel;
};
