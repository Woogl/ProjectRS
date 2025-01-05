// Copyright 2024 Team BH.


#include "RsProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

ARsProjectile::ARsProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}
