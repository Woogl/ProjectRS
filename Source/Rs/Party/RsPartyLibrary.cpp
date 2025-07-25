// Copyright 2024 Team BH.


#include "RsPartyLibrary.h"

#include "AbilitySystemComponent.h"
#include "RsPartyComponent.h"
#include "RsPartySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Player/RsPlayerController.h"

URsPartyComponent* URsPartyLibrary::GetPartyComponent(UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(PlayerController))
		{
			return RsPlayerController->GetPartyComponent();
		}
	}
	return nullptr;
}

ARsPlayerCharacter* URsPartyLibrary::GetPartyMemberAt(UObject* WorldContextObject, int32 MemberIndex)
{
	URsPartyComponent* PartyComponent = GetPartyComponent(WorldContextObject);
	if (!PartyComponent)
	{
		return nullptr;
	}
	return PartyComponent->GetPartyMember(MemberIndex);
}

TArray<ARsPlayerCharacter*> URsPartyLibrary::GetPartyMembers(UObject* WorldContextObject)
{
	URsPartyComponent* PartyComponent = GetPartyComponent(WorldContextObject);
	if (!PartyComponent)
	{
		return TArray<ARsPlayerCharacter*>();
	}
	return PartyComponent->GetPartyMembers();
}

int32 URsPartyLibrary::FindPartyMemberIndex(ARsPlayerCharacter* Member)
{
	TArray<ARsPlayerCharacter*> PartyMembers = GetPartyMembers(Member);
	return PartyMembers.Find(Member);
}

bool URsPartyLibrary::SwitchPartyMember(UObject* WorldContextObject, int32 NewMemberIndex)
{
	URsPartyComponent* PartyComponent = GetPartyComponent(WorldContextObject);
	if (!PartyComponent)
	{
		return false;
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	return PartyComponent->SwitchPartyMember(PlayerController, NewMemberIndex);
}

void URsPartyLibrary::AddPartyMember(ARsPlayerCharacter* NewMember)
{
	URsPartyComponent* PartyComponent = GetPartyComponent(NewMember);
	if (!PartyComponent)
	{
		return;
	}
	PartyComponent->AddPartyMember(NewMember);
}

void URsPartyLibrary::RemovePartyMember(ARsPlayerCharacter* MemberToRemove)
{
	URsPartyComponent* PartyComponent = GetPartyComponent(MemberToRemove);
	if (!PartyComponent)
	{
		return;
	}
	PartyComponent->RemovePartyMember(MemberToRemove);
}

void URsPartyLibrary::InsertPartyMemberAt(ARsPlayerCharacter* NewMember, int32 MemberIndex)
{
	URsPartyComponent* PartyComponent = GetPartyComponent(NewMember);
	if (!PartyComponent)
	{
		return;
	}
	PartyComponent->InsertPartyMemberAt(NewMember, MemberIndex);
}

void URsPartyLibrary::RemovePartyMemberAt(UObject* WorldContextObject, int32 MemberIndex)
{
	URsPartyComponent* PartyComponent = GetPartyComponent(WorldContextObject);
	if (!PartyComponent)
	{
		return;
	}
	PartyComponent->RemovePartyMemberAt(MemberIndex);
}

void URsPartyLibrary::JoinPartyMember(ULocalPlayer* LocalPlayer, TSubclassOf<ARsPlayerCharacter> NewMemberClass)
{
	if (URsPartySubsystem* PartySubsystem = URsPartySubsystem::Get(LocalPlayer))
	{
		PartySubsystem->JoinPartyMember(NewMemberClass);
	}
}

void URsPartyLibrary::LeavePartyMember(ULocalPlayer* LocalPlayer, TSubclassOf<ARsPlayerCharacter> MemberClassToRemove)
{
	if (URsPartySubsystem* PartySubsystem = URsPartySubsystem::Get(LocalPlayer))
	{
		PartySubsystem->LeavePartyMember(MemberClassToRemove);
	}
}

void URsPartyLibrary::ApplyPartyEffect(UObject* WorldContextObject, TSubclassOf<UGameplayEffect> EffectClass)
{
	for (ARsPlayerCharacter* PartyMember : GetPartyMembers(WorldContextObject))
	{
		if (UAbilitySystemComponent* ASC = PartyMember->GetAbilitySystemComponent())
		{
			UGameplayEffect* GameplayEffect = EffectClass->GetDefaultObject<UGameplayEffect>();
			FGameplayEffectSpec	Spec(GameplayEffect, ASC->MakeEffectContext(), 0);
			ASC->ApplyGameplayEffectSpecToSelf(Spec);
		}
	}
}

void URsPartyLibrary::ApplyPartyEffectSpec(UObject* WorldContextObject, FGameplayEffectSpec EffectSpec)
{
	for (ARsPlayerCharacter* PartyMember : GetPartyMembers(WorldContextObject))
	{
		if (UAbilitySystemComponent* ASC = PartyMember->GetAbilitySystemComponent())
		{
			ASC->ApplyGameplayEffectSpecToSelf(EffectSpec);
		}
	}
}

void URsPartyLibrary::ActivatePartyAbility(UObject* WorldContextObject, FGameplayTag AbilityTag)
{
	for (ARsPlayerCharacter* PartyMember : GetPartyMembers(WorldContextObject))
	{
		PartyMember->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
	}
}

void URsPartyLibrary::CancelPartyAbility(UObject* WorldContextObject, FGameplayTag AbilityTag)
{
	for (ARsPlayerCharacter* PartyMember : GetPartyMembers(WorldContextObject))
	{
		FGameplayTagContainer AbilityTags = AbilityTag.GetSingleTagContainer();
		PartyMember->GetAbilitySystemComponent()->CancelAbilities(&AbilityTags);
	}
}
