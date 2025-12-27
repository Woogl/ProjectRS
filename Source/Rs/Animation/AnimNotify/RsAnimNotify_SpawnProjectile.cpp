// Copyright 2024 Team BH.


#include "RsAnimNotify_SpawnProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Rs/Battle/Actor/RsProjectile.h"
#include "Rs/Targeting/RsTargetingLibrary.h"


URsAnimNotify_SpawnProjectile::URsAnimNotify_SpawnProjectile()
{
}

void URsAnimNotify_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	
	TArray<AActor*> Targets;
	URsTargetingLibrary::PerformTargetingInMeshSpace(MeshComp, TargetingParams, Targets);
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		if (UGameplayAbility* OwningAbility = ASC->GetAnimatingAbility())
		{
			FTransform SocketTransform = MeshComp->GetSocketTransform(SpawnSocketName);
			
			FGameplayAbilityTargetingLocationInfo StartInfo;
			StartInfo.LiteralTransform = SocketTransform;
			
			FGameplayAbilityTargetingLocationInfo EndInfo;
			if (Targets.IsValidIndex(0))
			{
				const FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(SocketTransform.GetLocation(), Targets[0]->GetActorLocation());
				FTransform EndTransform = Owner->GetActorTransform();
				EndTransform.SetRotation(LookRot.Quaternion());
				EndInfo.LiteralTransform = EndTransform;
			}
			else
			{
				FTransform EndTransform = Owner->GetActorTransform();
				EndTransform.AddToTranslation(EndTransform.GetUnitAxis(EAxis::X) * 40.f);
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
