// Copyright 2024 Team BH.


#include "RsPartyComponent.h"

#include "RsPartySubsystem.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AI/AIController/RsFriendlyAIController.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/Player/RsPlayerController.h"

URsPartyComponent::URsPartyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

ARsPlayerCharacter* URsPartyComponent::GetPartyMember(int32 MemberIndex) const
{
	if (PartyMembers.IsValidIndex(MemberIndex))
	{
		return PartyMembers[MemberIndex];
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("RsPartyComponent::GetPartyMember: Member Not Found"));
		return nullptr;
	}
}

TArray<ARsPlayerCharacter*> URsPartyComponent::GetPartyMembers() const
{
	return PartyMembers;
}

void URsPartyComponent::AddPartyMember(ARsPlayerCharacter* NewMember)
{
	if (!PartyMembers.Contains(NewMember))
	{
		PartyMembers.Add(NewMember);
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("RsPartyComponent::AddPartyMember: Already added member"));
	}
}

void URsPartyComponent::InsertPartyMember(ARsPlayerCharacter* NewMember, int32 MemberIndex)
{
	if (!PartyMembers.Contains(NewMember))
	{
		PartyMembers.Insert(NewMember, MemberIndex);
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("RsPartyComponent::InsertPartyMember: Already added member"));
	}
}

void URsPartyComponent::RemovePartyMember(ARsPlayerCharacter* MemberToRemove)
{
	if (PartyMembers.Contains(MemberToRemove))
	{
		PartyMembers.Remove(MemberToRemove);
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("RsPartyComponent::RemovePartyMember: Can't found Member"));
	}
}

bool URsPartyComponent::SwitchPartyMember(ARsPlayerController* PlayerController, int32 MemberIndex)
{
	if (ARsPlayerCharacter* NewPartyMember = GetPartyMember(MemberIndex))
	{
		if (PlayerController->GetPawn() != NewPartyMember)
		{
			if (PlayerController->GetPrevAIController())
			{
				PlayerController->GetPrevAIController()->Possess(PlayerController->GetPawn());
			}
			PlayerController->Possess(NewPartyMember);
			return true;
		}
		else
		{
			UE_LOG(RsLog, Warning, TEXT("RsPartyComponent::SwitchPartyMember: Can't switch to same character"));
		}
	}
	
	return false;
}

void URsPartyComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* OwningPlayerController = Cast<APlayerController>(GetOwner());
	if (OwningPlayerController == nullptr)
	{
		UE_LOG(RsLog, Warning, TEXT("RsPartyComponent's owner should be RsPlayerController"));
		return;
	}
	
	if (ULocalPlayer* LocalPlayer = OwningPlayerController->GetLocalPlayer())
	{
		for (TSubclassOf<ARsPlayerCharacter> PartyMemberClass : URsPartySubsystem::Get(LocalPlayer)->GetPartyMemberClasses())
		{
			if (ARsPlayerCharacter* SpawnedActor = GetWorld()->SpawnActor<ARsPlayerCharacter>(PartyMemberClass, GetOwner()->GetActorTransform()))
			{
				PartyMembers.Add(SpawnedActor);
			}
		}
	}
}
