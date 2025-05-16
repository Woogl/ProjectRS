// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCharacterViewModel.h"
#include "RsPlayerCharacterViewModel.generated.h"

class URsAbilityViewModel;
class URsEnergySetViewModel;
class ARsPlayerCharacter;
/**
 * 
 */
UCLASS()
class RS_API URsPlayerCharacterViewModel : public URsCharacterViewModel
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static URsPlayerCharacterViewModel* CreateRsPlayerCharacterViewModel(ARsPlayerCharacter* Model);
	
	int32 GetPartyMemberIndex() const;
	void SetPartyMemberIndex(int32 MemberIndex);

	bool GetIsPartyMember() const;
	void SetIsPartyMember(bool bPartyMember);

	bool GetIsPlayerControlled() const;
	void SetIsPlayerControlled(bool bPlayerControlled);

	UFUNCTION(FieldNotify, BlueprintPure)
	int32 GetPartySlotNumber() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetPartySlotNumberText() const;
	
protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

private:
	UFUNCTION()
	void HandlePossessedPawn(APawn* OldPawn, APawn* NewPawn);

	void HandleAddPartyMember(ARsPlayerCharacter* AddedMember, int32 MemberIndex);
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	int32 PartyMemberIndex;

	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	bool IsPartyMember = false;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, Getter, Setter, meta=(AllowPrivateAccess))
	bool IsPlayerControlled = false;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsEnergySetViewModel> EnergySetViewModel;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsAbilityViewModel> AbilityViewModel_E;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsAbilityViewModel> AbilityViewModel_Q;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsAbilityViewModel> AbilityViewModel_Ult;
};
