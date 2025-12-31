// Copyright 2024 Team BH.


#include "RsWeapon.h"

const FName ARsWeapon::DefaultWeaponID(TEXT("Weapon_Default"));

ARsWeapon::ARsWeapon()
{
	// Avoid ticking if possible.
	//PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	
	WeaponID = DefaultWeaponID;
}

FGenericTeamId ARsWeapon::GetGenericTeamId() const
{
	if (AActor* WeaponOwner = GetAttachParentActor())
	{
		if (IGenericTeamAgentInterface* TeamInterface = Cast<IGenericTeamAgentInterface>(WeaponOwner))
		{
			return TeamInterface->GetGenericTeamId();
		}
	}
	return FGenericTeamId::NoTeam;
}

FName ARsWeapon::GetWeaponID() const
{
	return WeaponID;
}

void ARsWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}
