// Copyright 2025 Team BH.


#include "RsAnimNotify_WeaponScan.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotify_WeaponScan::URsAnimNotify_WeaponScan()
{
	bIsNativeBranchingPoint = true;
}

FString URsAnimNotify_WeaponScan::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		FString EventTagString = EventTag.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
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
	
	FRsTargetingShape Shape;
	Shape.ShapeType = ShapeType;
	Shape.HalfExtent = WeaponComponent->Bounds.GetBox().GetExtent() * Offset.GetScale3D();
	
	FRsTargetingCollision Collision;
	Collision.CollisionObjectTypes = CollisionObjectTypes;
	Collision.CollisionChannels = CollisionChannels;
	
	FRsTargetingParams Params(Shape, Collision, Filter, Sorter);
	TArray<AActor*> ResultActors;
	if (URsTargetingLibrary::PerformTargeting(Owner, GetWeaponTransform(), Params, ResultActors))
	{
		UWorld* World = Owner->GetWorld();
		// Deal damage to each target.
		for (AActor* Target : ResultActors)
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
			{
				FGameplayEventData Payload;
				Payload.EventTag = EventTag;
				Payload.Instigator = Owner;
				Payload.Target = Target;
				FVector Start = WeaponComponent->GetComponentLocation();
				FVector End = Target->GetActorLocation();
				FHitResult HitResult;
				FCollisionQueryParams Query;
				Query.AddIgnoredActor(Owner);
				Owner->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_RsAttack, Query);
				URsTargetingLibrary::DrawDebugArrow(World, Start, End, FColor::Cyan);
				FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
				EffectContext.AddHitResult(HitResult);
				Payload.ContextHandle = EffectContext;
				ASC->HandleGameplayEvent(EventTag, &Payload);
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
