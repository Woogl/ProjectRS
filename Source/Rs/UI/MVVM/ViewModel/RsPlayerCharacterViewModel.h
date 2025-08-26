// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCharacterViewModel.h"
#include "RsPlayerCharacterViewModel.generated.h"

enum class ERsLinkSkillType : uint8;
class URsAbilityViewModel;
class URsEnergySetViewModel;
class ARsPlayerCharacter;
/**
 * 
 */
UCLASS()
class RS_API URsPlayerCharacterViewModel : public URsCharacterViewModel, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS | ViewModel")
	static URsPlayerCharacterViewModel* CreateRsPlayerCharacterViewModel(ARsPlayerCharacter* PlayerCharacter);
	
protected:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

public:
	UFUNCTION(FieldNotify, BlueprintPure)
	int32 GetPartyMemberIndex() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	int32 GetPartySlotNumber() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetPartySlotNumberText() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	bool IsPartyMember() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	bool IsPlayerControlled() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	ESlateVisibility GetDetailInfoVisibility() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetPartySwitchCooldownRemaining() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	bool CanActivateLinkSkill() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	ESlateVisibility GetLinkSkillVisibility() const;

	// FTickableGameObject
	// Display cooldown updated every frame.
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

private:
	UFUNCTION()
	void HandlePossessedPawn(APawn* OldPawn, APawn* NewPawn);

	void HandleAddPartyMember(ARsPlayerCharacter* AddedMember, int32 MemberIndex);
	void HandleRemovePartyMember(ARsPlayerCharacter* RemovedMember, int32 MemberIndex);
	void HandleLinkSkillReady(ARsCharacterBase* LinkSkillTarget, ERsLinkSkillType LinkSkillType, int32 LinkSkillCount);
	void HandleLinkSkillFinish(ERsLinkSkillType LinkSkillType);
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsEnergySetViewModel> EnergySetViewModel;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsAbilityViewModel> AbilityViewModel_E;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsAbilityViewModel> AbilityViewModel_Q;
	
	UPROPERTY(FieldNotify, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<URsAbilityViewModel> AbilityViewModel_Ult;
};
