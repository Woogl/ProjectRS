// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "RsCharacterViewModel.generated.h"

class UStaggerSetViewModel;
class UHealthSetViewModel;
class ARsCharacterBase;
/**
 * 
 */
UCLASS()
class RS_API URsCharacterViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static URsCharacterViewModel* CreateRsCharacterViewModel(ARsCharacterBase* Model);

	void Initialize();

	FText GetCharacterName() const;
	void SetCharacterName(FText NewCharacterName);
	
private:
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	FText CharacterName;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<UHealthSetViewModel> HealthSetViewModel;

	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<UStaggerSetViewModel> StaggerSetViewModel;
};
