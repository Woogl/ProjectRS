// Copyright 2024 Team BH.


#include "RsWeapon.h"

ARsWeapon::ARsWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bNetUseOwnerRelevancy = true;
}

void ARsWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}
