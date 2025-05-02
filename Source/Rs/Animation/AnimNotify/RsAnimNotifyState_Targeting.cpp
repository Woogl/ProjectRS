// Copyright 2024 Team BH.


#include "RsAnimNotifyState_Targeting.h"

#include "Engine/OverlapResult.h"
#include "Rs/AI/RsAILibrary.h"

URsAnimNotifyState_Targeting::URsAnimNotifyState_Targeting()
{
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = true;
#endif // WITH_EDITORONLY_DATA
}

void URsAnimNotifyState_Targeting::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Targets.Reset();
	PerformTargeting(MeshComp, MeshComp->GetSocketTransform(SocketName));

#if WITH_EDITOR
	SocketNames = MeshComp->GetAllSocketNames();
#endif // WITH_EDITOR
}

void URsAnimNotifyState_Targeting::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
}

bool URsAnimNotifyState_Targeting::PerformTargeting(const USkeletalMeshComponent* MeshComp, FTransform SourceTransform)
{
	bool bSuccess = false;
	if (MeshComp == nullptr)
	{
		return bSuccess;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
	{
		return bSuccess;
	}
	
	UWorld* World = Owner->GetWorld();
	if (World == nullptr)
	{
		return bSuccess;
	}
	
	SourceTransform.SetLocation(SourceTransform.GetLocation() + MeshComp->GetComponentTransform().TransformVector(PositionOffset));
	SourceTransform.SetRotation(SourceTransform.GetRotation() * RotationOffset.Quaternion());

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
	TArray<AActor*> ResultActors;
	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		if (OverlapResult.GetActor() == Owner)
		{
			continue;
		}
		if (bIgnoreFriendlyTeam)
		{
			if (URsAILibrary::GetTeamID(OverlapResult.GetActor()) == URsAILibrary::GetTeamID(Owner))
			{
				continue;
			}
		}
		if (bIgnoreEnemyTeam)
		{
			if (URsAILibrary::GetTeamID(OverlapResult.GetActor()) != URsAILibrary::GetTeamID(Owner))
			{
				continue;
			}
		}
		ResultActors.AddUnique(OverlapResult.GetActor());
	}

	// When AI blackboard has "TargetActor", use it.
	if (UObject* BBValue = URsAILibrary::GetBlackboardValueAsObject(Owner, TEXT("TargetActor")))
	{
		if (AActor* TargetActor = Cast<AActor>(BBValue))
		{
			ResultActors.Empty();
			ResultActors.Add(TargetActor);
		}
	}

	/** Sorting */
	if (bSortByDistance == true)
	{
		ResultActors.Sort([&Owner](const AActor& A, const AActor& B)
		{
			return FVector::Dist(A.GetActorLocation(), Owner->GetActorLocation()) < FVector::Dist(B.GetActorLocation(), Owner->GetActorLocation());
		});
	}
	
	/** Keep nearest actor */
	if (ResultActors.Num() > 0)
	{
		Targets.AddUnique(ResultActors[0]);
		bSuccess = true;
	}

#if WITH_EDITOR
	if (World->WorldType == EWorldType::PIE && bShowDebugInPIE)
	{
		DrawDebugShape(World, SourceTransform);
	}
	else if (World->WorldType == EWorldType::EditorPreview)
	{
		DrawDebugShape(World, SourceTransform);
	}
#endif // WITH_EDITOR

	return bSuccess;
}

FCollisionShape URsAnimNotifyState_Targeting::GetCollisionShape() const
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
void URsAnimNotifyState_Targeting::DrawDebugShape(const UWorld* World, FTransform SourceTransform)
{
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