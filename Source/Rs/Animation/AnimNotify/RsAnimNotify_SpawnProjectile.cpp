// Copyright 2024 Team BH.


#include "RsAnimNotify_SpawnProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "Rs/Battle/Actor/RsProjectile.h"

void URsAnimNotify_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			if (UGameplayAbility* CurrentAbility = OwnerASC->GetAnimatingAbility())
			{
				if (ACharacter* Character = Cast<ACharacter>(Owner))
				{
					FTransform SpawnTransform = Character->GetMesh()->GetSocketTransform(SocketName);
					SpawnTransform.SetRotation(Character->GetActorForwardVector().ToOrientationQuat());
					if (ARsProjectile* ProjectileInstance = Character->GetWorld()->SpawnActorDeferred<ARsProjectile>(ProjectileClass, SpawnTransform, Character, Character))
					{
						if (URsGameplayAbility_Attack* AttackAbility = Cast<URsGameplayAbility_Attack>(CurrentAbility))
						{
							if (FRsDamageContext* DamageContext = AttackAbility->FindDamageEvent(DamageEventTag))
							{
								ProjectileInstance->SetupDamage(AttackAbility, *DamageContext);
							}
						}
						ProjectileInstance->FinishSpawning(SpawnTransform);
					}
				}
			}
		}
	}
}
