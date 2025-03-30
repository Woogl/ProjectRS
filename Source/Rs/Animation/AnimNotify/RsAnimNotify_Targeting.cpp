// Copyright 2024 Team BH.


#include "RsAnimNotify_Targeting.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/OverlapResult.h"
#include "Rs/AI/RsAILibrary.h"

URsAnimNotify_Targeting::URsAnimNotify_Targeting()
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = true;
#endif // WITH_EDITORONLY_DATA
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
	SourceTransform.SetLocation(SourceTransform.GetLocation() + MeshComp->GetComponentTransform().TransformVector(PositionOffset));
	SourceTransform.SetRotation(SourceTransform.GetRotation() * RotationOffset.Quaternion() * SourceTransform.GetRotation());

	/** Overlapping */
	TArray<FOverlapResult> OverlapResults;
	if (CollisionObjectTypes.Num() > 0)
	{
		FCollisionObjectQueryParams ObjectParams;
		for (auto Iter = CollisionObjectTypes.CreateConstIterator(); Iter; ++Iter)
		{
			const ECollisionChannel& Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
			ObjectParams.AddObjectTypesToQuery(Channel);
		}
		World->OverlapMultiByObjectType(OverlapResults, SourceTransform.GetLocation(), SourceTransform.GetRotation(), ObjectParams, GetCollisionShape());
	}

	/** Filtering */
	ResultActors.Reset();
	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		if (AActor* ResultActor = OverlapResult.GetActor())
		{
			if (bIncludeSelf == false)
			{
				if (ResultActor == MeshComp->GetOwner())
				{
					continue;
				}
			}
			if (bIncludeFriendlyTeam == false)
			{
				if (URsAILibrary::GetTeamID(ResultActor) == URsAILibrary::GetTeamID(MeshComp->GetOwner()))
				{
					continue;
				}
			}
			if (bIncludeHostileTeam == false)
			{
				if (URsAILibrary::GetTeamID(ResultActor) != URsAILibrary::GetTeamID(MeshComp->GetOwner()))
				{
					continue;
				}
			}
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(ResultActor))
			{
				FGameplayTagContainer OutTags;
				TagInterface->GetOwnedGameplayTags(OutTags);
				if (TargetRequirements.RequirementsMet(OutTags) == false)
				{
					continue;
				}
			}
			ResultActors.AddUnique(ResultActor);
		}
	}

	/** Sorting */
	if (bSortByDistance)
	{
		ResultActors.Sort([&MeshComp](const AActor& A, const AActor& B)
		{
			return FVector::Dist(A.GetActorLocation(), MeshComp->GetComponentLocation()) < FVector::Dist(B.GetActorLocation(), MeshComp->GetComponentLocation());
		});
	}
	
	if (MaxTargetCount >= 1)
	{
		ResultActors.SetNum(MaxTargetCount);
	}

	/** Perform event for each result actor */
	if (ResultActors.Num() > 0)
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()))
		{
			for (AActor* ResultActor : ResultActors)
			{
				FGameplayEventData Payload;
				Payload.Instigator = MeshComp->GetOwner();
				Payload.Target = ResultActor;
				ASC->HandleGameplayEvent(EventTag, &Payload);
			}
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
	CachedMeshComp = MeshComp;
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
TArray<FName> URsAnimNotify_Targeting::GetSocketNames() const
{
	if (CachedMeshComp.IsValid())
	{
		return CachedMeshComp->GetAllSocketNames();
	}
	return TArray<FName>();
}

void URsAnimNotify_Targeting::DrawDebugShape(USkeletalMeshComponent* MeshComp, FTransform SourceTransform)
{
	const UWorld* World = MeshComp->GetWorld();
	const FCollisionShape CollisionShape = GetCollisionShape();

	const bool bPersistentLines = false;
	const uint8 DepthPriority = 0;
	const float Thickness = 1.0f;
	const FColor Color = FColor::Red;

	switch (ShapeType)
	{
	case ERsTargetingShape::Box:
		DrawDebugBox(World, SourceTransform.GetLocation(), CollisionShape.GetExtent(), SourceTransform.GetRotation(), Color, bPersistentLines, DebugLifeTime, DepthPriority, Thickness);
		break;
		
	case ERsTargetingShape::Sphere:
		DrawDebugCapsule(World, SourceTransform.GetLocation(), CollisionShape.GetSphereRadius(), CollisionShape.GetSphereRadius(), SourceTransform.GetRotation(), Color, bPersistentLines, DebugLifeTime, DepthPriority, Thickness);
		break;
		
	case ERsTargetingShape::Capsule:
		DrawDebugCapsule(World, SourceTransform.GetLocation(), CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), SourceTransform.GetRotation(), Color, bPersistentLines, DebugLifeTime, DepthPriority, Thickness);
		break;
		
	case ERsTargetingShape::Cylinder:
		const FVector RotatedExtent = SourceTransform.GetRotation() * CollisionShape.GetExtent();
		DrawDebugCylinder(World, SourceTransform.GetLocation() - RotatedExtent, SourceTransform.GetLocation() + RotatedExtent, CollisionShape.GetExtent().X, 16, Color, bPersistentLines, DebugLifeTime, DepthPriority, Thickness);
		break;
	}
}
#endif // WITH_EDITOR