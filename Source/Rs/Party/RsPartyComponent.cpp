// Copyright 2024 Team BH.


#include "RsPartyComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "RsPartySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/RsLogChannels.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/UI/RsUILibrary.h"

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
		URsUILibrary::AddSystemMessage(GetWorld(), FText::FromString(TEXT("Member Not Found")));
		return nullptr;
	}
}

TArray<ARsPlayerCharacter*> URsPartyComponent::GetPartyMembers() const
{
	return PartyMembers;
}

int32 URsPartyComponent::GetPartyMemberIndex(ARsPlayerCharacter* Character) const
{
	return PartyMembers.Find(Character);
}

int32 URsPartyComponent::GetCurrentMemberIndex() const
{
	if (ACharacter* CurrentCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		return GetPartyMemberIndex(Cast<ARsPlayerCharacter>(CurrentCharacter));
	}
	return INDEX_NONE;
}

ARsPlayerCharacter* URsPartyComponent::GetCurrentPartyMember() const
{
	int32 CurrentIndex = GetCurrentMemberIndex();
	return PartyMembers[CurrentIndex];
}

ARsPlayerCharacter* URsPartyComponent::GetPrevPartyMember() const
{
	int32 CurrentIndex = GetCurrentMemberIndex();
	int32 PrevIndex = ((CurrentIndex + PartyMembers.Num()) - 1) % PartyMembers.Num();
	return PartyMembers[PrevIndex];
}

ARsPlayerCharacter* URsPartyComponent::GetNextPartyMember() const
{
	int32 CurrentIndex = GetCurrentMemberIndex();
	int32 NextIndex = ((CurrentIndex + PartyMembers.Num()) + 1) % PartyMembers.Num();
	return PartyMembers[NextIndex];
}

ARsPlayerCharacter* URsPartyComponent::GetPrevAlivePartyMember() const
{
	int32 MemberSize = PartyMembers.Num();
	int32 CurrentIndex = GetCurrentMemberIndex();
	for (int32 i = 1; i < MemberSize; ++i)
	{
		int32 TargetIndex = (CurrentIndex - i + MemberSize) % MemberSize;
		ARsPlayerCharacter* Member = GetPartyMember(TargetIndex);
		if (!URsBattleLibrary::IsDead(Member))
		{
			return Member;
		}
	}
	return nullptr;
}

ARsPlayerCharacter* URsPartyComponent::GetNextAlivePartyMember() const
{
	int32 MemberSize = PartyMembers.Num();
	int32 CurrentIndex = GetCurrentMemberIndex();
	for (int32 i = 1; i < MemberSize; ++i)
	{
		int32 TargetIndex = (CurrentIndex + i) % MemberSize;
		ARsPlayerCharacter* Member = GetPartyMember(TargetIndex);
		if (!URsBattleLibrary::IsDead(Member))
		{
			return Member;
		}
	}
	return nullptr;
}

void URsPartyComponent::AddPartyMember(ARsPlayerCharacter* NewMember)
{
	if (!PartyMembers.Contains(NewMember))
	{
		int32 MemberIndex = PartyMembers.Add(NewMember);
		OnAddPartyMember.Broadcast(NewMember, MemberIndex);
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("RsPartyComponent::AddPartyMember: Already added member"));
		URsUILibrary::AddSystemMessage(GetWorld(), FText::FromString(TEXT("Already added member")));
	}
}

void URsPartyComponent::RemovePartyMember(ARsPlayerCharacter* MemberToRemove)
{
	if (PartyMembers.Contains(MemberToRemove))
	{
		int32 MemberIndex = PartyMembers.Find(MemberToRemove);
		PartyMembers.Remove(MemberToRemove);
		OnRemovePartyMember.Broadcast(MemberToRemove, MemberIndex);
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("RsPartyComponent::RemovePartyMember: Can't found Member"));
		URsUILibrary::AddSystemMessage(GetWorld(), FText::FromString(TEXT("Can't found Member")));
	}
}

void URsPartyComponent::InsertPartyMemberAt(ARsPlayerCharacter* NewMember, int32 MemberIndex)
{
	if (!PartyMembers.Contains(NewMember))
	{
		PartyMembers.Insert(NewMember, MemberIndex);
		for (int32 i = MemberIndex; i < PartyMembers.Num(); i++)
		{
			OnAddPartyMember.Broadcast(PartyMembers[i], i);
		}
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("RsPartyComponent::InsertPartyMember: Already added member"));
		URsUILibrary::AddSystemMessage(GetWorld(), FText::FromString(TEXT("Already added member")));
	}
}

void URsPartyComponent::RemovePartyMemberAt(int32 MemberIndex)
{
	if (PartyMembers.IsValidIndex(MemberIndex))
	{
		ARsPlayerCharacter* RemovedCharacter = PartyMembers[MemberIndex];
		PartyMembers.RemoveAtSwap(MemberIndex);
		OnRemovePartyMember.Broadcast(RemovedCharacter, MemberIndex);
	}
	else
	{
		UE_LOG(RsLog, Warning, TEXT("RsPartyComponent::RemovePartyMemberAt: Can't found Member"));
		URsUILibrary::AddSystemMessage(GetWorld(), FText::FromString(TEXT("Can't found Member")));
	}
}

void URsPartyComponent::SpawnPartyMembers()
{
	APlayerController* OwingPlayerController = Cast<APlayerController>(GetOwner());
	if (ULocalPlayer* LocalPlayer = OwingPlayerController->GetLocalPlayer())
	{
		for (TSubclassOf<ARsPlayerCharacter> PartyMemberClass : URsPartySubsystem::Get(LocalPlayer)->GetPartyMemberClasses())
		{
			if (ARsPlayerCharacter* SpawnedActor = GetWorld()->SpawnActor<ARsPlayerCharacter>(PartyMemberClass, GetOwner()->GetActorTransform()))
			{
				AddPartyMember(SpawnedActor);
			}
		}
	}
}

bool URsPartyComponent::SwitchPartyMember(APlayerController* PlayerController, int32 MemberIndex)
{
	if (ARsPlayerCharacter* NewPartyMember = GetPartyMember(MemberIndex))
	{
		if (PlayerController->GetPawn() != NewPartyMember)
		{
			if (URsBattleLibrary::IsDead(NewPartyMember) == true)
			{
				UE_LOG(RsLog, Warning, TEXT("RsPartyComponent::SwitchPartyMember: Can't switch to dead character"));
				URsUILibrary::AddSystemMessage(GetWorld(), FText::FromString(TEXT("Can't switch to dead character")));
				return false;
			}
			
			PlayerController->Possess(NewPartyMember);
			return true;
		}
		else
		{
			UE_LOG(RsLog, Warning, TEXT("RsPartyComponent::SwitchPartyMember: Can't switch to same character"));
			URsUILibrary::AddSystemMessage(GetWorld(), FText::FromString(TEXT("Can't switch to same character")));
		}
	}
	
	return false;
}

bool URsPartyComponent::TrySwitchMemberAbility(int32 MemberIndex)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		return false;
	}
	
	if (APawn* ControllingPawn = PlayerController->GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ControllingPawn))
		{
			if (SwitchPartyMemberAbilityTags.IsValidIndex(MemberIndex))
			{
				FGameplayTag AbilityTag = SwitchPartyMemberAbilityTags[MemberIndex];
				return ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
			}
		}
	}
	return false;
}

AActor* URsPartyComponent::GetLinkSkillTarget() const
{
	return LinkSkillTarget;
}

void URsPartyComponent::SetLinkSkillTarget(AActor* NewTarget)
{
	LinkSkillTarget = NewTarget;
}

void URsPartyComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnPartyMembers();
}
