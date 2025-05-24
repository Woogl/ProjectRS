// Copyright 2024 Team BH.


#include "RsGameplayAbility_Ranged.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Rs/Battle/Actor/RsProjectile.h"
#include "Rs/Character/RsCharacterBase.h"

void URsGameplayAbility_Ranged::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ProjectileClass)
	{
		if (FireEventTag != FGameplayTag::EmptyTag)
		{
			UAbilityTask_WaitGameplayEvent* FireProjectileTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FireEventTag);
			FireProjectileTask->EventReceived.AddDynamic(this, &ThisClass::HandleFireProjectile);
			FireProjectileTask->ReadyForActivation();
		}
	}
}

void URsGameplayAbility_Ranged::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CachedVictim.Reset();
}

void URsGameplayAbility_Ranged::HandleFireProjectile(FGameplayEventData EventData)
{
	ARsCharacterBase* Source = GetAvatarCharacter();

	FTransform ProjectileTransform = Source->GetActorTransform();
	if (CachedVictim.IsValid())
	{
		FVector Start = Source->GetActorLocation();
		FVector End = CachedVictim->GetActorLocation();
		ProjectileTransform.SetRotation(UKismetMathLibrary::FindLookAtRotation(Start, End).Quaternion());
	}

	ARsProjectile* Projectile = GetWorld()->SpawnActorDeferred<ARsProjectile>(ProjectileClass, ProjectileTransform, Source, Source, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (FRsDamageContext* DamageContext = DamageEvents.FindByKey(EventData.EventTag))
	{
		Projectile->SetupDamage(this, *DamageContext);
	}
	
	if (CachedVictim.IsValid())
	{
		Projectile->ProjectileMovement->HomingTargetComponent = CachedVictim.Get()->GetRootComponent();
	}
	
	Projectile->FinishSpawning(AvatarCharacter->GetActorTransform());
}
