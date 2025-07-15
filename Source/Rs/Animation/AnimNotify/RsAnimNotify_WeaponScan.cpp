// Copyright 2025 Team BH.


#include "RsAnimNotify_WeaponScan.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotify_WeaponScan::URsAnimNotify_WeaponScan()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_WeaponScan::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp)
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	USceneComponent* WeaponComponent = Owner->FindComponentByTag<USceneComponent>(ComponentTag);
	if (!WeaponComponent)
	{
		return;
	}
	
	FTransform WeaponTransform = WeaponComponent->GetComponentTransform();
	WeaponTransform.SetLocation(WeaponComponent->Bounds.Origin);
	
	FRsTargetingCollision Collision;
	Collision.ShapeType = ShapeType;
	Collision.CollisionObjectTypes = CollisionObjectTypes;
	Collision.HalfExtent = WeaponComponent->Bounds.GetBox().GetExtent() * Scale;
	
	TArray<AActor*> ResultActors;
	if (URsTargetingLibrary::PerformTargeting(Owner, WeaponTransform, Collision, Filter, Sorter, ResultActors))
	{
		// Deal damage to each target.
		for (AActor* Target : ResultActors)
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
			{
				FGameplayEventData Payload;
				Payload.EventTag = DamageEvent;
				Payload.Instigator = Owner;
				Payload.Target = Target;
				ASC->HandleGameplayEvent(DamageEvent, &Payload);
			}
		}
	}
}
