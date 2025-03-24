// Copyright 2024 Team BH.


#include "RsCharacterBase.h"

#include "Net/UnrealNetwork.h"
#include "Rs/AbilitySystem/Component/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/Component/RsHealthComponent.h"
#include "Rs/AbilitySystem/Component/RsStaggerComponent.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/UI/Component/RsNameplateComponent.h"

ARsCharacterBase::ARsCharacterBase()
{
	HealthComponent = CreateDefaultSubobject<URsHealthComponent>(TEXT("HealthComponent"));
	StaggerComponent = CreateDefaultSubobject<URsStaggerComponent>(TEXT("StaggerComponent"));
	
	NameplateComponent = CreateDefaultSubobject<URsNameplateComponent>(TEXT("NameplateComponent"));
	NameplateComponent->SetupAttachment(RootComponent);
	NameplateComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
}

void ARsCharacterBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamId);
}

UAbilitySystemComponent* ARsCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

URsAbilitySystemComponent* ARsCharacterBase::GetRsAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARsCharacterBase::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	uint8 Id = InTeamID;
	TeamId = static_cast<ERsTeamId>(Id);
}

FGenericTeamId ARsCharacterBase::GetGenericTeamId() const
{
	return static_cast<uint8>(TeamId);
}

bool ARsCharacterBase::IsLockableTarget_Implementation() const
{
	if (URsBattleLibrary::IsDead(this))
	{
		return false;
	}
	return true;
}
