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
#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Party/RsPartyComponent.h"
#include "Rs/Party/RsPartyLibrary.h"
#include "Rs/System/RsGameSettingDataAsset.h"

URsPlayerCharacterViewModel* URsPlayerCharacterViewModel::CreateRsPlayerCharacterViewModel(ARsPlayerCharacter* PlayerCharacter)
{
	return CreateViewModel<URsPlayerCharacterViewModel>(PlayerCharacter);
}

void URsPlayerCharacterViewModel::Initialize()
{
	Super::Initialize();
	if (ARsCharacterBase* PlayerCharacter = GetModel<ThisClass>())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerCharacter))
		{
			if (const URsEnergySet* EnergySet = ASC->GetSet<URsEnergySet>())
			{
				UE_MVVM_SET_PROPERTY_VALUE(EnergySetViewModel, URsEnergySetViewModel::CreateEnergySetViewModel(EnergySet));
			}
			if (URsGameplayAbility* Skill_E = URsAbilitySystemLibrary::FindRsAbilityWithTag(ASC, URsGameSettingDataAsset::Get()->ESkillTag.GetSingleTagContainer(), true))
			{
				UE_MVVM_SET_PROPERTY_VALUE(AbilityViewModel_E, URsAbilityViewModel::CreateRsAbilityViewModel(Skill_E));
			}
			if (URsGameplayAbility* Skill_Q = URsAbilitySystemLibrary::FindRsAbilityWithTag(ASC, URsGameSettingDataAsset::Get()->QSkillTag.GetSingleTagContainer(), true))
			{
				UE_MVVM_SET_PROPERTY_VALUE(AbilityViewModel_Q, URsAbilityViewModel::CreateRsAbilityViewModel(Skill_Q));
			}
			if (URsGameplayAbility* Skill_Ult = URsAbilitySystemLibrary::FindRsAbilityWithTag(ASC, URsGameSettingDataAsset::Get()->UltSkillTag.GetSingleTagContainer(), true))
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

		// Listen link skill ready
		if (URsBattleSubsystem* BattleSubsystem = URsBattleSubsystem::Get(this))
		{
			BattleSubsystem->OnLinkSkillReady.AddUObject(this, &ThisClass::HandleLinkSkillReady);
			BattleSubsystem->OnLinkSkillFinish.AddUObject(this, &ThisClass::HandleLinkSkillFinish);
		}
	}
}

void URsPlayerCharacterViewModel::Deinitialize()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetModel<ThisClass>(), 0))
	{
		PlayerController->OnPossessedPawnChanged.RemoveAll(this);
		if (URsPartyComponent* PartyComponent = PlayerController->FindComponentByClass<URsPartyComponent>())
		{
			PartyComponent->OnAddPartyMember.RemoveAll(this);
			PartyComponent->OnRemovePartyMember.RemoveAll(this);
		}
	}
	
	if (URsBattleSubsystem* BattleSubsystem = URsBattleSubsystem::Get(this))
	{
		BattleSubsystem->OnLinkSkillReady.RemoveAll(this);
		BattleSubsystem->OnLinkSkillFinish.RemoveAll(this);
	}
	Super::Deinitialize();
}

int32 URsPlayerCharacterViewModel::GetPartyMemberIndex() const
{
	if (ARsPlayerCharacter* Character = GetModel<ThisClass>())
	{
		TArray<ARsPlayerCharacter*> PartyMembers = URsPartyLibrary::GetPartyMembers(GetModel<ThisClass>());
		return PartyMembers.Find(Cast<ARsPlayerCharacter>(Character));
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
	if (ARsPlayerCharacter* PlayerCharacter = GetModel<ThisClass>())
	{
		return UGameplayStatics::GetPlayerController(GetWorld(), 0) == PlayerCharacter->GetController();
	}
	return false;
}

ESlateVisibility URsPlayerCharacterViewModel::GetDetailInfoVisibility() const
{
	if (HealthSetViewModel && HealthSetViewModel->IsDead() || IsPlayerControlled())
	{
		return ESlateVisibility::Hidden;
	}
	return ESlateVisibility::HitTestInvisible;
}

FText URsPlayerCharacterViewModel::GetPartySwitchCooldownRemaining() const
{
	if (ARsCharacterBase* PlayerCharacter = GetModel<ThisClass>())
	{
		if (UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent())
		{
			FGameplayTag CooldownTag = URsGameSettingDataAsset::Get()->SwitchMemberCooldownTag;
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

bool URsPlayerCharacterViewModel::CanActivateLinkSkill() const
{
	if (IsPlayerControlled())
	{
		return false;
	}
	URsBattleSubsystem* BattleSubsystem = URsBattleSubsystem::Get(GetModel<ThisClass>());
	if (!BattleSubsystem || !BattleSubsystem->IsLinkSkillReady())
	{
		return false;
	}
	if (UAbilitySystemComponent* ASC = GetModel<ThisClass>()->GetAbilitySystemComponent())
	{
		FGameplayTagContainer LinkSkillTag = URsGameSettingDataAsset::Get()->LinkSkillTag.GetSingleTagContainer();
		if (UGameplayAbility* LinkSkillAbility = URsAbilitySystemLibrary::FindAbilityWithTag(ASC, LinkSkillTag, false))
		{
			if (FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(LinkSkillAbility->GetClass()))
			{
				return LinkSkillAbility->CanActivateAbility(Spec->Handle, LinkSkillAbility->GetCurrentActorInfo());
			}
		}
	}
	return false;
}

ESlateVisibility URsPlayerCharacterViewModel::GetLinkSkillVisibility() const
{
	if (CanActivateLinkSkill())
	{
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Collapsed;
}

void URsPlayerCharacterViewModel::OnSwitchButtonPressed()
{
	PlayerPawnSwitchRequestEvent.Execute(GetPartyMemberIndex());
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
	ARsCharacterBase* Character = GetModel<ThisClass>();
	if (!Character)
	{
		return;
	}
	
	if (OldPawn == Character || NewPawn == Character)
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsPlayerControlled);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDetailInfoVisibility);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPartySwitchCooldownRemaining);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CanActivateLinkSkill);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLinkSkillVisibility);
	}
}

void URsPlayerCharacterViewModel::HandleAddPartyMember(ARsPlayerCharacter* AddedMember, int32 MemberIndex)
{
	if (AddedMember == GetModel<ThisClass>())
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
	if (RemovedMember == GetModel<ThisClass>())
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

void URsPlayerCharacterViewModel::HandleLinkSkillReady(ARsCharacterBase* LinkSkillTarget, ERsLinkSkillType LinkSkillType, int32 LinkSkillCount)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CanActivateLinkSkill);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLinkSkillVisibility);
}

void URsPlayerCharacterViewModel::HandleLinkSkillFinish(ERsLinkSkillType LinkSkillType)
{
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CanActivateLinkSkill);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLinkSkillVisibility);
}
