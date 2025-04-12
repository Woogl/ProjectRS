// Copyright 2024 Team BH.


#include "RsPartyLibrary.h"

#include "AbilitySystemComponent.h"
#include "RsPartyComponent.h"
#include "RsPartySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Player/RsPlayerController.h"

ARsPlayerCharacter* URsPartyLibrary::GetPartyMemberAt(UObject* WorldContextObject, int32 MemberIndex)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		return RsPlayerController->GetPartyComponent()->GetPartyMember(MemberIndex);
	}
	
	return nullptr;
}

TArray<ARsPlayerCharacter*> URsPartyLibrary::GetPartyMembers(UObject* WorldContextObject)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		return RsPlayerController->GetPartyComponent()->GetPartyMembers();
	}
	
	return TArray<ARsPlayerCharacter*>();
}

bool URsPartyLibrary::SwitchPartyMember(UObject* WorldContextObject, int32 NewMemberIndex)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		return RsPlayerController->GetPartyComponent()->SwitchPartyMember(RsPlayerController, NewMemberIndex);
	}
	
	return false;
}

void URsPartyLibrary::AddPartyMember(ARsPlayerCharacter* NewMember)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(NewMember, 0)))
	{
		RsPlayerController->GetPartyComponent()->AddPartyMember(NewMember);
	}
}

void URsPartyLibrary::RemovePartyMember(ARsPlayerCharacter* MemberToRemove)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(MemberToRemove, 0)))
	{
		RsPlayerController->GetPartyComponent()->RemovePartyMember(MemberToRemove);
	}
}

void URsPartyLibrary::InsertPartyMemberAt(ARsPlayerCharacter* NewMember, int32 MemberIndex)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(NewMember, 0)))
	{
		RsPlayerController->GetPartyComponent()->InsertPartyMemberAt(NewMember, MemberIndex);
	}
}

void URsPartyLibrary::RemovePartyMemberAt(UObject* WorldContextObject, int32 MemberIndex)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		RsPlayerController->GetPartyComponent()->RemovePartyMemberAt(MemberIndex);
	}
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
