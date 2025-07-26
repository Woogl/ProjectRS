// Copyright 2024 Team BH.


#include "RsPlayerCharacterViewModel.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "RsAbilityViewModel.h"
#include "RsEnergySetViewModel.h"
#include "RsHealthSetViewModel.h"
#include "Components/SlateWrapperTypes.h"
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

bool URsPlayerCharacterViewModel::TrySwitchMember()
{
	ARsCharacterBase* Model = CachedModel.Get();
	if (!Model)
	{
		return false;
	}
	URsPartyComponent* PartyComponent = URsPartyLibrary::GetPartyComponent(this);
	if (!PartyComponent)
	{
		return false;
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		return false;
	}
	
	int32 MemberIndex = GetPartyMemberIndex();
	return PartyComponent->TrySwitchPartyMember(PlayerController, MemberIndex);
}

void URsPlayerCharacterViewModel::Initialize()
{
	Super::Initialize();
	
	if (ARsCharacterBase* Model = CachedModel.Get())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Model))
		{
			EnergySetViewModel = URsEnergySetViewModel::CreateEnergySetViewModel(ASC);
			
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

		// Listen party member change
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			PlayerController->OnPossessedPawnChanged.AddUniqueDynamic(this, &ThisClass::HandlePossessedPawn);
			if (URsPartyComponent* PartyComponent = PlayerController->FindComponentByClass<URsPartyComponent>())
			{
				PartyComponent->OnAddPartyMember.AddUObject(this, &ThisClass::HandleAddPartyMember);
				PartyComponent->OnRemovePartyMember.AddUObject(this, &ThisClass::HandleRemovePartyMember);
			}
		}
	}
}

void URsPlayerCharacterViewModel::Deinitialize()
{
	Super::Deinitialize();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(CachedModel.Get(), 0))
	{
		PlayerController->OnPossessedPawnChanged.RemoveAll(this);
		if (URsPartyComponent* PartyComponent = PlayerController->FindComponentByClass<URsPartyComponent>())
		{
			PartyComponent->OnAddPartyMember.RemoveAll(this);
			PartyComponent->OnRemovePartyMember.RemoveAll(this);
		}
	}
}

int32 URsPlayerCharacterViewModel::GetPartyMemberIndex() const
{
	if (ARsPlayerCharacter* Model = Cast<ARsPlayerCharacter>(GetOuter()))
	{
		TArray<ARsPlayerCharacter*> PartyMembers = URsPartyLibrary::GetPartyMembers(CachedModel.Get());
		return PartyMembers.Find(Cast<ARsPlayerCharacter>(Model));
	}
	return INDEX_NONE;
}

int32 URsPlayerCharacterViewModel::GetPartySlotNumber() const
{
	if (GetPartyMemberIndex() == INDEX_NONE)
	{
		return INDEX_NONE;
	}
	return GetPartyMemberIndex() + 1;
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
		return UGameplayStatics::GetPlayerController(GetWorld(), 0) == Model->GetController();
	}
	return false;
}

ESlateVisibility URsPlayerCharacterViewModel::GetDetailInfoVisibility() const
{
	if (HealthSetViewModel->IsDead() || IsPlayerControlled())
	{
		return ESlateVisibility::Hidden;
	}
	return ESlateVisibility::HitTestInvisible;
}

FText URsPlayerCharacterViewModel::GetPartySwitchCooldownRemaining() const
{
	if (ARsCharacterBase* Model = CachedModel.Get())
	{
		if (UAbilitySystemComponent* ASC = Model->GetAbilitySystemComponent())
		{
			FGameplayTag CooldownTag = URsGameSetting::Get()->SwitchMemberCooldownTag;
			FGameplayEffectQuery EffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(CooldownTag.GetSingleTagContainer());
			TArray<float> TimeRemaining = ASC->GetActiveEffectsTimeRemaining(EffectQuery);
			if (TimeRemaining.IsValidIndex(0))
			{
				static const FNumberFormattingOptions Format = FNumberFormattingOptions().SetMinimumFractionalDigits(1).SetMaximumFractionalDigits(1);
				return FText::AsNumber(TimeRemaining[0], &Format);
			}
		}
	}
	return FText::GetEmpty();
}

void URsPlayerCharacterViewModel::Tick(float DeltaTime)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartySwitchCooldownRemaining);
}

TStatId URsPlayerCharacterViewModel::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URsPlayerCharacterViewModel, STATGROUP_Tickables);
}

void URsPlayerCharacterViewModel::HandlePossessedPawn(APawn* OldPawn, APawn* NewPawn)
{
	if (OldPawn == GetOuter() || NewPawn == GetOuter())
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsPlayerControlled);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDetailInfoVisibility);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartySwitchCooldownRemaining);
	}
}

void URsPlayerCharacterViewModel::HandleAddPartyMember(ARsPlayerCharacter* AddedMember, int32 MemberIndex)
{
	if (AddedMember == GetOuter())
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartyMemberIndex);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartySlotNumber);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartySlotNumberText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsPlayerControlled);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsPartyMember);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDetailInfoVisibility);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartySwitchCooldownRemaining);
	}
}

void URsPlayerCharacterViewModel::HandleRemovePartyMember(ARsPlayerCharacter* RemovedMember, int32 MemberIndex)
{
	if (RemovedMember == GetOuter())
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartyMemberIndex);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartySlotNumber);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartySlotNumberText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsPlayerControlled);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsPartyMember);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDetailInfoVisibility);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartySwitchCooldownRemaining);
	}
}
