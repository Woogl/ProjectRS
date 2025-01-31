// Copyright 2024 Team BH.


#include "RsAnimNotify_Targeting.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/OverlapResult.h"
#include "Rs/AI/RsAILibrary.h"

URsAnimNotify_Targeting::URsAnimNotify_Targeting()
{
	bShouldFireInEditor = true;
}

void URsAnimNotify_Targeting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		return;
	}
	
	UWorld* World = MeshComp->GetWorld();
	if (World == nullptr)
	{
		return;
	}
	
	FTransform SourceTransform = SocketName.IsValid() ? MeshComp->GetSocketTransform(SocketName) : MeshComp->GetComponentTransform();
	SourceTransform.SetLocation(SourceTransform.GetLocation() + MeshComp->GetComponentTransform().TransformVector(Offset));

	/** Find targeting result actors */
	TArray<AActor*> ResultActors;
	if (CollisionObjectTypes.Num() > 0)
	{
		FCollisionObjectQueryParams ObjectParams;
		for (auto Iter = CollisionObjectTypes.CreateConstIterator(); Iter; ++Iter)
		{
			const ECollisionChannel& Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
			ObjectParams.AddObjectTypesToQuery(Channel);
		}
		TArray<FOverlapResult> OverlapResults;
		World->OverlapMultiByObjectType(OverlapResults, SourceTransform.GetLocation(), SourceTransform.GetRotation(), ObjectParams, GetCollisionShape());
		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			ResultActors.AddUnique(OverlapResult.GetActor());
		}
	}

	/** Perform event for each result actor */
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()))
	{
		if (bSortByDistance)
		{
			ResultActors.Sort([&MeshComp](const AActor& A, const AActor& B)
			{
				return FVector::Dist(A.GetActorLocation(), MeshComp->GetComponentLocation()) < FVector::Dist(B.GetActorLocation(), MeshComp->GetComponentLocation());
			});
		}
			
		for (AActor* ResultActor : ResultActors)
		{
			if (bPreventSelfDamage)
			{
				if (ResultActor == MeshComp->GetOwner())
				{
					continue;
				}
			}
				
			if (bPreventTeamDamage)
			{
				if (URsAILibrary::GetTeamID(ResultActor) == URsAILibrary::GetTeamID(MeshComp->GetOwner()))
				{
					continue;
				}
			}
				
			FGameplayEventData Payload;
			Payload.Target = ResultActor;
			ASC->HandleGameplayEvent(EventTag, &Payload);
		}
	}

#if WITH_EDITOR
	if (World->WorldType == EWorldType::PIE && bShowDebugInPIE)
	{
		DrawDebugShape(MeshComp, SourceTransform);
	}
	else if (World->WorldType == EWorldType::EditorPreview)
	{
		DrawDebugShape(MeshComp, SourceTransform);
	}
#endif // WITH_EDITOR
}

FCollisionShape URsAnimNotify_Targeting::GetCollisionShape() const
{
	switch (ShapeType)
	{
	case ERsTargetingShape::Box:
		return FCollisionShape::MakeBox(HalfExtent);

	case ERsTargetingShape::Cylinder:
		return FCollisionShape::MakeBox(HalfExtent);

	case ERsTargetingShape::Sphere:
		return FCollisionShape::MakeSphere(Radius);

	case ERsTargetingShape::Capsule:
		return FCollisionShape::MakeCapsule(Radius, HalfHeight);
	}
	
	return FCollisionShape();
}

#if WITH_EDITOR
void URsAnimNotify_Targeting::DrawDebugShape(USkeletalMeshComponent* MeshComp, FTransform SourceTransform)
{
	const UWorld* World = MeshComp->GetWorld();
	const FCollisionShape CollisionShape = GetCollisionShape();

	const bool bPersistentLines = false;
	const float LifeTime = 0.5f;
	const uint8 DepthPriority = 0;
	const float Thickness = 1.0f;
	const FColor Color = FColor::Red;

	switch (ShapeType)
	{
	case ERsTargetingShape::Box:
		DrawDebugBox(World, SourceTransform.GetLocation(), CollisionShape.GetExtent(), SourceTransform.GetRotation(), Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
		
	case ERsTargetingShape::Sphere:
		DrawDebugCapsule(World, SourceTransform.GetLocation(), CollisionShape.GetSphereRadius(), CollisionShape.GetSphereRadius(), SourceTransform.GetRotation(), Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
		
	case ERsTargetingShape::Capsule:
		DrawDebugCapsule(World, SourceTransform.GetLocation(), CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), SourceTransform.GetRotation(), Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
		
	case ERsTargetingShape::Cylinder:
		const FVector RotatedExtent = SourceTransform.GetRotation() * CollisionShape.GetExtent();
		DrawDebugCylinder(World, SourceTransform.GetLocation() - RotatedExtent, SourceTransform.GetLocation() + RotatedExtent, CollisionShape.GetExtent().X, 16, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
	}
}
#endif // WITH_EDITOR