// Copyright 2024 Team BH.


#include "RsPlayerCharacterViewModel.h"

#include "AbilitySystemGlobals.h"
#include "RsAbilityViewModel.h"
#include "RsEnergySetViewModel.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/System/RsGameSetting.h"

URsPlayerCharacterViewModel* URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(ARsPlayerCharacter* Model)
{
	URsPlayerCharacterViewModel* ViewModel = NewObject<URsPlayerCharacterViewModel>(Model);
	ViewModel->Initialize();
	return ViewModel;
}

void URsPlayerCharacterViewModel::Initialize()
{
	Super::Initialize();
	
	if (ARsPlayerCharacter* Model = Cast<ARsPlayerCharacter>(GetOuter()))
	{
		EnergySetViewModel = URsEnergySetViewModel::CreateEnergySetViewModel(Model);
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Model))
		{
			if (URsGameplayAbility* Skill_E = URsAbilitySystemLibrary::FindRsAbilityWithTag(ASC, URsGameSetting::Get()->ESkillTag.GetSingleTagContainer(), true))
			{
				UE_MVVM_SET_PROPERTY_VALUE(AbilityViewModel_E, URsAbilityViewModel::CreateRsAbilityViewModel(Skill_E));
			}
			if (URsGameplayAbility* Skill_Q = URsAbilitySystemLibrary::FindRsAbilityWithTag(ASC, URsGameSetting::Get()->QSkillTag.GetSingleTagContainer(), true))
			{
				UE_MVVM_SET_PROPERTY_VALUE(AbilityViewModel_Q, URsAbilityViewModel::CreateRsAbilityViewModel(Skill_Q));
			}
			if (URsGameplayAbility* Skill_Ult = URsAbilitySystemLibrary::FindRsAbilityWithTag(ASC, URsGameSetting::Get()->UltSkillTag.GetSingleTagContainer(), true))
			{
				UE_MVVM_SET_PROPERTY_VALUE(AbilityViewModel_Ult, URsAbilityViewModel::CreateRsAbilityViewModel(Skill_Ult));
			}
		}

		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			PlayerController->OnPossessedPawnChanged.AddUniqueDynamic(this, &ThisClass::HandlePossessedPawn);
			if (URsPartyComponent* PartyComponent = PlayerController->FindComponentByClass<URsPartyComponent>())
			{
				PartyComponent->OnAddPartyMember.AddUObject(this, &ThisClass::HandleAddPartyMember);
				int32 MemberIndex = PartyComponent->GetPartyMembers().Find(Model);
				SetPartyMemberIndex(MemberIndex);
			}
		}
	}
}

void URsPlayerCharacterViewModel::Deinitialize()
{
	Super::Deinitialize();

	if (ARsPlayerCharacter* Model = Cast<ARsPlayerCharacter>(GetOuter()))
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(Model, 0))
		{
			PlayerController->OnPossessedPawnChanged.RemoveAll(this);
			if (URsPartyComponent* PartyComponent = PlayerController->FindComponentByClass<URsPartyComponent>())
			{
				PartyComponent->OnAddPartyMember.RemoveAll(this);
			}
		}
	}
}

int32 URsPlayerCharacterViewModel::GetPartyMemberIndex() const
{
	return PartyMemberIndex;
}

void URsPlayerCharacterViewModel::SetPartyMemberIndex(int32 MemberIndex)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(PartyMemberIndex, MemberIndex))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartySlotNumber);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartySlotNumberText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsPlayerControlled);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsPartyMember);
	}
}

int32 URsPlayerCharacterViewModel::GetPartySlotNumber() const
{
	return PartyMemberIndex + 1;
}

FText URsPlayerCharacterViewModel::GetPartySlotNumberText() const
{
	return FText::AsNumber(GetPartySlotNumber());
}

bool URsPlayerCharacterViewModel::IsPartyMember() const
{
	TArray<ARsPlayerCharacter*> PartyMembers = URsPartyLibrary::GetPartyMembers(GetWorld());
	if (PartyMembers.Num() > 0)
	{
		return PartyMembers.Contains(GetOuter());
	}
	return false;
}

bool URsPlayerCharacterViewModel::IsPlayerControlled() const
{
	if (ARsPlayerCharacter* Model = Cast<ARsPlayerCharacter>(GetOuter()))
	{
		return Model->IsLocallyControlled();
	}
	return false;
}

void URsPlayerCharacterViewModel::HandlePossessedPawn(APawn* OldPawn, APawn* NewPawn)
{
	if (OldPawn == GetOuter() || NewPawn == GetOuter())
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsPlayerControlled);
	}
}

void URsPlayerCharacterViewModel::HandleAddPartyMember(ARsPlayerCharacter* AddedMember, int32 MemberIndex)
{
	if (AddedMember == GetOuter())
	{
		SetPartyMemberIndex(MemberIndex);
	}
}