// Copyright 2024 Team BH.


#include "RsAnimNotify_SpawnProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Rs/Battle/Actor/RsProjectile.h"


URsAnimNotify_SpawnProjectile::URsAnimNotify_SpawnProjectile()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	PerformTargeting(MeshComp);
	
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			if (UGameplayAbility* OwningAbility = ASC->GetAnimatingAbility())
			{
				FGameplayAbilityTargetingLocationInfo StartInfo;
				StartInfo.LiteralTransform = MeshComp->GetSocketTransform(SocketName);
				
				FGameplayAbilityTargetingLocationInfo EndInfo;
				if (Targets.IsValidIndex(0))
				{
					EndInfo.LiteralTransform = Targets[0]->GetActorTransform();
				}
				else
				{
					FTransform EndTransform = Owner->GetActorTransform();
					EndTransform.AddToTranslation(EndTransform.GetUnitAxis(EAxis::X) * 100.f);
					EndInfo.LiteralTransform = EndTransform;
				}
				
				FGameplayAbilityTargetDataHandle TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromLocations(StartInfo, EndInfo);
				
				if (UAbilityTask_SpawnActor* SpawnTask = UAbilityTask_SpawnActor::SpawnActor(OwningAbility, TargetData, ProjectileClass))
				{
					AActor* SpawnedActor;
					SpawnTask->BeginSpawningActor(OwningAbility, TargetData, ProjectileClass, SpawnedActor);
					if (SpawnedActor)
					{
						SpawnedActor->SetInstigator(Cast<APawn>(ASC->GetOwnerActor()));
						if (ARsProjectile* Projectile = Cast<ARsProjectile>(SpawnedActor))
						{
							Projectile->OwningAbility = OwningAbility;
							if (Projectile->ProjectileMovement->bIsHomingProjectile && Targets.IsValidIndex(0))
							{
								Projectile->ProjectileMovement->HomingTargetComponent = Targets[0]->GetRootComponent();
							}
						}
					}
					SpawnTask->FinishSpawningActor(OwningAbility, TargetData, SpawnedActor);
				}
			}
		}
	}
}
