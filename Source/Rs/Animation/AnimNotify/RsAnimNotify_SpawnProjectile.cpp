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
	
	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character))
	{
		if (UGameplayAbility* CurrentAbility = OwnerASC->GetAnimatingAbility())
		{
			if (Targets.Num() == 0 && bFireAtLeastOne)
			{
				Targets.Add(nullptr);
			}
				
			for (AActor* Target : Targets)
			{
				FTransform SpawnTransform = !SpawnSocketName.IsNone() ? Character->GetMesh()->GetSocketTransform(SpawnSocketName) : Character->GetActorTransform();
				const FVector TargetLocation = Target != nullptr ? Target->GetActorLocation() : Character->GetActorLocation() + Character->GetActorForwardVector() * 300.f;
				if (ARsProjectile* ProjectileInstance = Character->GetWorld()->SpawnActorDeferred<ARsProjectile>(ProjectileClass, SpawnTransform, Character, Character))
				{
					ProjectileInstance->SetupDamage(Cast<URsGameplayAbility_Attack>(CurrentAbility), DamageEventTag);
							
					switch (ProjectileInstance->Direction)
					{
					case ERsProjectileDirection::SourceForward:
						SpawnTransform.SetRotation(Character->GetActorForwardVector().ToOrientationQuat());
						break;
								
					case ERsProjectileDirection::SourceToTarget:
						FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnTransform.GetLocation(), TargetLocation);
						SpawnTransform.SetRotation(SpawnRotation.Quaternion());
						break;
								
					case ERsProjectileDirection::SkyToTarget:
						FVector SpawnLocation = TargetLocation + FVector(0, 0, ProjectileInstance->SpawnHeight);
						SpawnTransform.SetLocation(SpawnLocation);
						SpawnTransform.SetRotation((TargetLocation - SpawnLocation).GetSafeNormal().ToOrientationQuat());
						break;
								
					default:
						UE_LOG(RsLog, Error, TEXT("%s's direction is unknown!"), *ProjectileInstance->GetName());
						break;
					}
					
					ProjectileInstance->FinishSpawning(SpawnTransform);
				}
			}
		}
	}
}
