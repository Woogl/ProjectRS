// Copyright 2024 Team BH.


#include "RsPartyComponent.h"

#include "RsPartySubsystem.h"
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
		UE_LOG(LogTemp, Warning, TEXT("RsPartyComponent::GetPartyMember: Member Not Found"));
		return nullptr;
	}
}

void URsPartyComponent::AddPartyMember(ARsPlayerCharacter* NewMember)
{
	if (!PartyMembers.Contains(NewMember))
	{
		PartyMembers.Add(NewMember);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RsPartyComponent::AddPartyMember: Already added member"));
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
		UE_LOG(LogTemp, Warning, TEXT("RsPartyComponent::RemovePartyMember: Can't found Member"));
	}
}

void URsPartyComponent::SwitchPartyMember(ARsPlayerController* PlayerController, int32 MemberIndex)
{
	if (ARsPlayerCharacter* NewPartyMember = GetPartyMember(MemberIndex))
	{
		if (PlayerController->GetPawn() != NewPartyMember)
		{
			if (PlayerController->GetPrevController() == nullptr)
			{
				if (PlayerController->GetPawn()->AIControllerClass)
				{
					if (AActor* PrevController = PlayerController->GetWorld()->SpawnActor(PlayerController->GetPawn()->AIControllerClass))
					{
						PlayerController->SetPrevController(Cast<AController>(PrevController));
					}
				}
			}
			if (PlayerController->GetPrevController())
			{
				PlayerController->GetPrevController()->Possess(PlayerController->GetPawn());
			}
			PlayerController->Possess(NewPartyMember);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RsPartyComponent::SwitchPartyMember: Can't switch to same character"));
		}
	}
}

void URsPartyComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* OwningPlayerController = Cast<APlayerController>(GetOwner());
	if (OwningPlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("RsPartyComponent's owner should be RsPlayerController"));
		return;
	}
	
	if (ULocalPlayer* LocalPlayer = OwningPlayerController->GetLocalPlayer())
	{
		for (TSubclassOf<ARsPlayerCharacter> PartyMemberClass : URsPartySubsystem::Get(LocalPlayer)->GetPartyMemberClasses())
		{
			ARsPlayerCharacter* SpawnedActor = GetWorld()->SpawnActor<ARsPlayerCharacter>(PartyMemberClass, GetOwner()->GetActorTransform());
			PartyMembers.Add(SpawnedActor);
		}
	}
	
	if (!PartyMembers.IsEmpty())
	{
		OwningPlayerController->Possess(PartyMembers[0]);
	}
}
