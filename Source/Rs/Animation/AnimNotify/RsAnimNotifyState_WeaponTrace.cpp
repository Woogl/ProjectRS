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

	LastTransform = GetWeaponTransform();
	
	Shape.ShapeType = ShapeType;
	Shape.HalfExtent = WeaponComponent->GetLocalBounds().GetBox().GetExtent() * Offset.GetScale3D();
	Collision.CollisionObjectTypes = CollisionObjectTypes;
	Collision.CollisionChannels = CollisionChannels;

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
	if (URsTargetingLibrary::PerformTargetingWithSubsteps(Owner, LastTransform, GetWeaponTransform(), MaxSubsteps, Shape, Collision, Filter, Sorter, ResultActors))
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
	FTransform WorldTransform = WeaponComponent->GetComponentTransform();
	FTransform OffsetNoScale = Offset;
	OffsetNoScale.SetScale3D(FVector::OneVector);
	WorldTransform *= OffsetNoScale;
	WorldTransform.SetLocation(WeaponComponent->Bounds.Origin + WorldTransform.TransformVector(Offset.GetLocation()));
	return WorldTransform;
}
