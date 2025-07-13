// Copyright 2025 Team BH.


#include "RsAnimNotifyState_WeaponTrace.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotifyState_WeaponTrace::URsAnimNotifyState_WeaponTrace()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotifyState_WeaponTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp)
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	WeaponComponent = Owner->FindComponentByTag<UMeshComponent>(ComponentTag);
	if (!WeaponComponent.IsValid())
	{
		return;
	}
	
	LastTransform = WeaponComponent->GetComponentTransform();
	Collision.ShapeType = ShapeType;
	Collision.CollisionObjectTypes = CollisionObjectTypes;
	Collision.HalfExtent = WeaponComponent->GetLocalBounds().GetBox().GetExtent();

	HitTargets.Empty();
}

void URsAnimNotifyState_WeaponTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	if (!WeaponComponent.IsValid())
	{
		return;
	}

	FTransform CurrentTransform = WeaponComponent->GetComponentTransform();
	TArray<AActor*> ResultActors;
	if (URsTargetingLibrary::PerformTargetingWithSubsteps(Owner, LastTransform, CurrentTransform, MaxSubsteps, Collision, Filter, Sorter, ResultActors))
	{
		// Deal damage to each target.
		for (AActor* Target : ResultActors)
		{
			if (!HitTargets.Contains(Target))
			{
				if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
				{
					FGameplayEventData Payload;
					Payload.EventTag = DamageEvent;
					Payload.Instigator = Owner;
					Payload.Target = Target;
					ASC->HandleGameplayEvent(DamageEvent, &Payload);
				}
				HitTargets.Emplace(Target);
			}
		}
	}

	// Keep old transform for next tick.
	LastTransform = CurrentTransform;
}
