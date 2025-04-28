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
	PerformTargeting(MeshComp);
}

void URsAnimNotifyState_Targeting::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
}

void URsAnimNotifyState_Targeting::PerformTargeting(USkeletalMeshComponent* MeshComp)
{
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
	TArray<AActor*> ResultActors;
	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		if (OverlapResult.GetActor() == MeshComp->GetOwner())
		{
			continue;
		}
		if (bIgnoreFriendlyTeam)
		{
			if (URsAILibrary::GetTeamID(OverlapResult.GetActor()) == URsAILibrary::GetTeamID(MeshComp->GetOwner()))
			{
				continue;
			}
		}
		if (bIgnoreEnemyTeam)
		{
			if (URsAILibrary::GetTeamID(OverlapResult.GetActor()) != URsAILibrary::GetTeamID(MeshComp->GetOwner()))
			{
				continue;
			}
		}
		ResultActors.AddUnique(OverlapResult.GetActor());
	}

	// When AI blackboard has "TargetActor", use it.
	if (UObject* BBValue = URsAILibrary::GetBlackboardValueAsObject(MeshComp->GetOwner(), TEXT("TargetActor")))
	{
		if (AActor* TargetActor = Cast<AActor>(BBValue))
		{
			ResultActors.Empty();
			ResultActors.Add(TargetActor);
		}
	}

	/** Sorting */
	ResultActors.Sort([&MeshComp](const AActor& A, const AActor& B)
	{
		return FVector::Dist(A.GetActorLocation(), MeshComp->GetComponentLocation()) < FVector::Dist(B.GetActorLocation(), MeshComp->GetComponentLocation());
	});
	
	/** Keep nearest actor */
	if (ResultActors.Num() > 0)
	{
		Targets.AddUnique(ResultActors[0]);
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
TArray<FName> URsAnimNotifyState_Targeting::GetSocketNames() const
{
	if (CachedMeshComp.IsValid())
	{
		return CachedMeshComp->GetAllSocketNames();
	}
	return TArray<FName>();
}

void URsAnimNotifyState_Targeting::DrawDebugShape(USkeletalMeshComponent* MeshComp, FTransform SourceTransform)
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