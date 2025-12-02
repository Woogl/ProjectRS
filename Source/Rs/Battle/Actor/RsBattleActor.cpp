// Copyright 2025 Team BH.


#include "RsBattleActor.h"

#include "Net/UnrealNetwork.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"


ARsBattleActor::ARsBattleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void ARsBattleActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamId);
}

void ARsBattleActor::BeginPlay()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		for (URsAbilitySet* AbilitySet : AbilitySets)
		{
			AbilitySystemComponent->InitializeAbilitySet(AbilitySet);
		}
	}
	
	Super::BeginPlay();
}

UAbilitySystemComponent* ARsBattleActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARsBattleActor::GetOwnedGameplayTags(FGameplayTagContainer& OutTagContainer) const
{
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer OwnedTags;
		AbilitySystemComponent->GetOwnedGameplayTags(OwnedTags);
		OutTagContainer = MoveTemp(OwnedTags);
	}
}

void ARsBattleActor::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	uint8 Id = InTeamID;
	TeamId = static_cast<ERsTeamId>(Id);
}

FGenericTeamId ARsBattleActor::GetGenericTeamId() const
{
	return static_cast<uint8>(TeamId);
}
