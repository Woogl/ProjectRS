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
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static URsCharacterViewModel* CreateRsCharacterViewModel(ARsCharacterBase* Model);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

public:
	FText GetCharacterName() const;
	UObject* GetCharacterIcon() const;
    	
	void SetCharacterName(FText NewCharacterName);
	void SetCharacterIcon(UObject* NewCharacterIcon);

	UFUNCTION(FieldNotify, BlueprintPure)
	bool HasCharacterIcon() const;
	
private:
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	FText CharacterName;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	TObjectPtr<UObject> CharacterIcon;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsHealthSetViewModel> HealthSetViewModel;

	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsStaggerSetViewModel> StaggerSetViewModel;
};
