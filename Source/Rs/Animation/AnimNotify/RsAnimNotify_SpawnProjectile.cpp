// Copyright 2024 Team BH.


#include "RsAnimNotify_SpawnProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/Battle/Actor/RsProjectile.h"

void URsAnimNotify_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !ProjectileClass)
	{
		return;
	}
	
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character)
	{
		return;
	}

	UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character);
	if (!OwnerASC)
	{
		return;
	}

	UGameplayAbility* CurrentAbility = OwnerASC->GetAnimatingAbility();
	if (!CurrentAbility)
	{
		return;
	}
	
	if (Targets.Num() == 0 && bFireAtLeastOne)
	{
		Targets.Add(nullptr);
	}
			
	const FVector CharacterForward = Character->GetActorForwardVector();
	for (AActor* Target : Targets)
	{
		FVector SpawnLocation = !SpawnSocketName.IsNone() ? Character->GetMesh()->GetSocketLocation(SpawnSocketName) : Character->GetActorLocation();
		FVector TargetLocation = Target ? Target->GetActorLocation() : SpawnLocation + CharacterForward * DefaultTargetDistance;

		if (ARsProjectile* Projectile = Character->GetWorld()->SpawnActorDeferred<ARsProjectile>(ProjectileClass, FTransform(), Character, Character))
		{
			Projectile->SetupDamage(Cast<URsGameplayAbility>(CurrentAbility), DamageEventTag);
			FRotator SpawnRotation;

			switch (Projectile->Direction)
			{
			case ERsProjectileDirection::SourceForward:
				SpawnRotation = CharacterForward.Rotation();
				break;

			case ERsProjectileDirection::SourceToTarget:
				SpawnRotation = (TargetLocation - SpawnLocation).GetSafeNormal().Rotation();
				break;

			case ERsProjectileDirection::SkyToTarget:
				SpawnLocation = TargetLocation + FVector(0, 0, Projectile->SpawnHeight) + CharacterForward * (Projectile->FallbackSpawnDistance - DefaultTargetDistance);
				SpawnRotation = (TargetLocation - SpawnLocation).GetSafeNormal().Rotation();
				break;

			default:
				UE_LOG(RsLog, Error, TEXT("Unknown projectile direction: %s"), *Projectile->GetName());
				continue;
			}

			Projectile->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
		}
	}
}
