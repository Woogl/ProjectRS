// Copyright 2024 Team BH.


#include "RsProjectile.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility_Attack.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Targeting/RsTargetingLibrary.h"


ARsProjectile::ARsProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBeginOverlap);
	Capsule->OnComponentHit.AddDynamic(this, &ThisClass::HandleBlock);
	Capsule->SetCollisionProfileName(TEXT("RsProjectile"));
	SetRootComponent(Capsule);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
}

void ARsProjectile::SetupDamage(URsGameplayAbility_Attack* InOwningAbility, FGameplayTag InDamageEventTag)
{
	OwningAbility = InOwningAbility;
	DamageEventTag = InDamageEventTag;
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
	TArray<AActor*> InActors;
	InActors.Add(OtherActor);
	TArray<AActor*> FilteredActor = URsTargetingLibrary::PerformFiltering(InActors, GetInstigator(), DamageFilter);
	
	if (GetInstigator() && FilteredActor.Contains(OtherActor))
	{
		FRsDamageContext* DamageContext = OwningAbility->FindDamageEvent(DamageEventTag);
		URsBattleLibrary::ApplyDamageContext(GetInstigator(), OtherActor, *DamageContext);
		OwningAbility->OnAttackHitTarget(OtherActor, DamageEventTag);
		
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