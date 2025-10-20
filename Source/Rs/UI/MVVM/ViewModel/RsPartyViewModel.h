// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsSingletonViewModelBase.h"
#include "RsPartyViewModel.generated.h"

class URsPlayerCharacterViewModel;
class URsPartyComponent;
class ARsPlayerCharacter;
/**
 * 
 */
UCLASS()
class RS_API URsPartyViewModel : public URsSingletonViewModelBase
{
	GENERATED_BODY()

public:
	using ModelType = URsPartyComponent;
	
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel", meta=(WorldContext="WorldContext"))
	static URsPartyViewModel* GetPartyViewModel(UObject* WorldContext);

	void TrySwitchMemberAbility(int32 MemberIndex);
	
	virtual void Initialize() override;
	virtual void Deinitialize() override;

public:
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
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TArray<TObjectPtr<URsPlayerCharacterViewModel>> PartyMemberViewModels;
};
