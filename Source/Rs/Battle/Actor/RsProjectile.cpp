// Copyright 2024 Team BH.


#include "RsProjectile.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility_Attack.h"
#include "Rs/AI/RsAILibrary.h"
#include "Rs/Battle/RsBattleLibrary.h"


ARsProjectile::ARsProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBeginOverlap);
	Capsule->OnComponentHit.AddDynamic(this, &ThisClass::HandleBlock);
	SetRootComponent(Capsule);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
}

void ARsProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileMovement->MaxSpeed != 0)
	{
		SetLifeSpan(MaxRange / ProjectileMovement->MaxSpeed);
	}
}

void ARsProjectile::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (DamageContext.EffectCoefficients.IsEmpty())
	// {
	// 	return;
	// }

	if (bCannotHitFriend == true)
	{
		if (URsAILibrary::GetTeamID(GetInstigator()) == URsAILibrary::GetTeamID(OtherActor))
		{
			return;
		}
	}

	if (OwningAbility.IsValid())
	{
		OwningAbility->ApplyCostRecovery();
		OwningAbility->OnAttackHitTarget(OtherActor, EventTag);
	}
	
	if (GetInstigator() && OtherActor)
	{
		URsBattleLibrary::ApplyDamageContext(GetInstigator(), OtherActor, DamageContext);
		MaxHitCount--;
		if (MaxHitCount == 0)
		{
			Destroy();
		}
	}
}

void ARsProjectile::HandleBlock(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}