// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsViewModelBase.h"
#include "RsPartyViewModel.generated.h"

class URsPlayerCharacterViewModel;
class URsPartyComponent;
class ARsPlayerCharacter;
/**
 * 
 */
UCLASS()
class RS_API URsPartyViewModel : public URsViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel", meta=(WorldContext="WorldContext"))
	static URsPartyViewModel* GetPartyViewModel(UObject* WorldContext);

	UFUNCTION(BlueprintCallable)
	bool TrySwitchMemberAbility(int32 MemberIndex);
	
	virtual void Initialize() override;
	virtual void Deinitialize() override;

public:
	UFUNCTION(FieldNotify, BlueprintPure)
	ESlateVisibility GetPartyMemberVisibility_0() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	ESlateVisibility GetPartyMemberVisibility_1() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	ESlateVisibility GetPartyMemberVisibility_2() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	URsPlayerCharacterViewModel* GetCurrentPartyMember() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	URsPlayerCharacterViewModel* GetPrevPartyMember() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	URsPlayerCharacterViewModel* GetNextPartyMember() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	URsPlayerCharacterViewModel* GetPrevAlivePartyMember() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	URsPlayerCharacterViewModel* GetNextAlivePartyMember() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	int32 GetCurrentPartyMemberIndex() const;

private:
	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
	void HandleAddPartyMember(ARsPlayerCharacter* PartyMember, int32 MemberIndex);
	void HandleRemovePartyMember(ARsPlayerCharacter* PartyMember, int32 MemberIndex);

	void SyncPartyMembers(int32 Index);
	
	void SetPartyMemberViewModel_0(URsPlayerCharacterViewModel* CharacterViewModel);
	void SetPartyMemberViewModel_1(URsPlayerCharacterViewModel* CharacterViewModel);
	void SetPartyMemberViewModel_2(URsPlayerCharacterViewModel* CharacterViewModel);
	
	UPROPERTY(FieldNotify, Setter, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsPlayerCharacterViewModel> PartyMemberViewModel_0;

	UPROPERTY(FieldNotify, Setter, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsPlayerCharacterViewModel> PartyMemberViewModel_1;

	UPROPERTY(FieldNotify, Setter, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsPlayerCharacterViewModel> PartyMemberViewModel_2;
};
