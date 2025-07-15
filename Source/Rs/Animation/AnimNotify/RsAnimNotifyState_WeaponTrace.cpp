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
	WeaponComponent = Owner->FindComponentByTag<USceneComponent>(ComponentTag);
	if (!WeaponComponent.IsValid())
	{
		return;
	}

	LastTransform = GetWeaponTransform();
	
	Collision.ShapeType = ShapeType;
	Collision.CollisionObjectTypes = CollisionObjectTypes;
	Collision.HalfExtent = WeaponComponent->GetLocalBounds().GetBox().GetExtent() * Scale;

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
	
	TArray<AActor*> ResultActors;
	if (URsTargetingLibrary::PerformTargetingWithSubsteps(Owner, LastTransform, GetWeaponTransform(), MaxSubsteps, Collision, Filter, Sorter, ResultActors))
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
	LastTransform = GetWeaponTransform();
}

FTransform URsAnimNotifyState_WeaponTrace::GetWeaponTransform() const
{
	if (!WeaponComponent.IsValid())
	{
		return FTransform::Identity;
	}
	FTransform CurrentTransform = WeaponComponent->GetComponentTransform();
	CurrentTransform.SetLocation(WeaponComponent->Bounds.Origin);
	return CurrentTransform;
}
