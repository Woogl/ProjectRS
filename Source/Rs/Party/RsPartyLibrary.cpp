// Copyright 2024 Team BH.


#include "RsPartyLibrary.h"

#include "AbilitySystemComponent.h"
#include "RsPartyComponent.h"
#include "RsPartySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Player/RsPlayerController.h"

bool URsPartyLibrary::SwitchPartyMember(UObject* WorldContextObject, int32 NewMemberIndex)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		return RsPlayerController->GetPartyComponent()->SwitchPartyMember(RsPlayerController, NewMemberIndex);
	}
	
	return false;
}

void URsPartyLibrary::ApplyPartyEffect(UObject* WorldContextObject, TSubclassOf<UGameplayEffect> EffectClass)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		for (ARsPlayerCharacter* PartyMember : RsPlayerController->GetPartyComponent()->GetPartyMembers())
		{
			if (UAbilitySystemComponent* ASC = PartyMember->GetAbilitySystemComponent())
			{
				UGameplayEffect* GameplayEffect = EffectClass->GetDefaultObject<UGameplayEffect>();
				FGameplayEffectSpec	Spec(GameplayEffect, ASC->MakeEffectContext(), 0);
				ASC->ApplyGameplayEffectSpecToSelf(Spec);
			}
		}
	}
}

void URsPartyLibrary::ApplyPartyEffectSpec(UObject* WorldContextObject, FGameplayEffectSpec& EffectSpec)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		for (ARsPlayerCharacter* PartyMember : RsPlayerController->GetPartyComponent()->GetPartyMembers())
		{
			if (UAbilitySystemComponent* ASC = PartyMember->GetAbilitySystemComponent())
			{
				ASC->ApplyGameplayEffectSpecToSelf(EffectSpec);
			}
		}
	}
}

void URsPartyLibrary::AddPartyMember(ARsPlayerCharacter* NewMember)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(NewMember, 0)))
	{
		RsPlayerController->GetPartyComponent()->AddPartyMember(NewMember);
	}
}

void URsPartyLibrary::InsertPartyMember(ARsPlayerCharacter* NewMember, int32 MemberIndex)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(NewMember, 0)))
	{
		RsPlayerController->GetPartyComponent()->InsertPartyMember(NewMember, MemberIndex);
	}
}

void URsPartyLibrary::RemovePartyMember(ARsPlayerCharacter* MemberToRemove)
{
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(UGameplayStatics::GetPlayerController(MemberToRemove, 0)))
	{
		RsPlayerController->GetPartyComponent()->RemovePartyMember(MemberToRemove);
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
