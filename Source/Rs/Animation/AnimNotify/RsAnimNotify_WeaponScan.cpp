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
	
	switch (WeaponRecognition)
	{
	case ERsWeaponRecognitionMethod::ByMeshComponent:
		WeaponComponent = Owner->FindComponentByTag<USceneComponent>(WeaponTag);
		break;
		
	case ERsWeaponRecognitionMethod::ByWeaponClass:
		TArray<AActor*> OutActors;
		Owner->GetAttachedActors(OutActors);
		if (OutActors.IsEmpty())
		{
			return;
		}
		for (AActor* Actor : OutActors)
		{
			if (Actor->ActorHasTag(WeaponTag))
			{
				WeaponComponent = Actor->GetRootComponent();
				break;
			}
		}
		break;
	}
	
	if (!WeaponComponent.IsValid())
	{
		return;
	}
	
	FRsTargetingCollision Collision;
	Collision.ShapeType = ShapeType;
	Collision.CollisionObjectTypes = CollisionObjectTypes;
	Collision.HalfExtent = WeaponComponent->Bounds.GetBox().GetExtent() * Offset.GetScale3D();
	
	TArray<AActor*> ResultActors;
	if (URsTargetingLibrary::PerformTargeting(Owner, GetWeaponTransform(), Collision, Filter, Sorter, ResultActors))
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

FTransform URsAnimNotify_WeaponScan::GetWeaponTransform() const
{
	if (!WeaponComponent.IsValid())
	{
		return FTransform::Identity;
	}
	FTransform WorldTransform = WeaponComponent->GetComponentTransform();
	FTransform OffsetNoScale = Offset;
	OffsetNoScale.SetScale3D(FVector(1,1,1));
	WorldTransform *= OffsetNoScale;
	WorldTransform.SetLocation(WeaponComponent->Bounds.Origin + WorldTransform.TransformVector(Offset.GetLocation()));
	return WorldTransform;
}
