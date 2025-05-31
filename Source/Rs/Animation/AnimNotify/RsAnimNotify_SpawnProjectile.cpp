// Copyright 2024 Team BH.


#include "RsAnimNotify_SpawnProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Rs/RsLogChannels.h"
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
	const float DefaultDistance = 100.f;

	for (AActor* Target : Targets)
	{
		const FVector SpawnLocation = !SpawnSocketName.IsNone() ? Character->GetMesh()->GetSocketLocation(SpawnSocketName) : Character->GetActorLocation();
		const FVector TargetLocation = Target ? Target->GetActorLocation() : SpawnLocation + CharacterForward * DefaultDistance;

		if (ARsProjectile* Projectile = Character->GetWorld()->SpawnActorDeferred<ARsProjectile>(ProjectileClass, FTransform(), Character, Character))
		{
			Projectile->SetupDamage(Cast<URsGameplayAbility_Attack>(CurrentAbility), DamageEventTag);

			FVector FinalLocation = SpawnLocation;
			FRotator FinalRotation;

			switch (Projectile->Direction)
			{
			case ERsProjectileDirection::SourceForward:
				FinalRotation = CharacterForward.Rotation();
				break;

			case ERsProjectileDirection::SourceToTarget:
				FinalRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);
				break;

			case ERsProjectileDirection::SkyToTarget:
				FinalLocation = TargetLocation + FVector(0, 0, Projectile->SpawnHeight) + CharacterForward * (Projectile->FallbackSpawnDistance - DefaultDistance);
				FinalRotation = (TargetLocation - FinalLocation).GetSafeNormal().Rotation();
				break;

			default:
				UE_LOG(RsLog, Error, TEXT("Unknown projectile direction: %s"), *Projectile->GetName());
				continue;
			}

			Projectile->FinishSpawning(FTransform(FinalRotation, FinalLocation));
		}
	}
}
