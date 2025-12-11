// Copyright 2024 Team BH.


#include "RsAnimNotify_SpawnProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_SpawnPredictedProjectile.h"

URsAnimNotify_SpawnProjectile::URsAnimNotify_SpawnProjectile()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
			{
				if (UGameplayAbility* OwningAbility = ASC->GetAnimatingAbility())
				{
					FTransform SpawnTransform = MeshComp->GetSocketTransform(SpawnSocketName);
					URsAbilityTask_SpawnPredictedProjectile* Task = URsAbilityTask_SpawnPredictedProjectile::SpawnPredictedProjectile(OwningAbility, ProjectileClass, SpawnTransform.GetLocation(), SpawnTransform.Rotator());
					Task->ReadyForActivation();
				}
			}
		}
	}
}
