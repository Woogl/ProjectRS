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
	UFUNCTION(BlueprintCallable)
	static URsPartyViewModel* CreateRsPartyViewModel(URsPartyComponent* PartyComponent);

protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	TWeakObjectPtr<URsPartyComponent> CachedModel;

public:
	UFUNCTION(FieldNotify, BlueprintPure)
	ESlateVisibility GetPartyMemberVisibility_0() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	ESlateVisibility GetPartyMemberVisibility_1() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	ESlateVisibility GetPartyMemberVisibility_2() const;

private:
	void HandleAddPartyMember(ARsPlayerCharacter* PartyMember, int32 MemberIndex);
	void HandleRemovePartyMember(ARsPlayerCharacter* PartyMember, int32 MemberIndex);

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
