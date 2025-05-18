// Copyright 2024 Team BH.


#include "RsAnimNotifyState_Targeting.h"

#include "GameplayTagAssetInterface.h"
#include "Engine/OverlapResult.h"
#include "Rs/AI/RsAILibrary.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotifyState_Targeting::URsAnimNotifyState_Targeting()
{
	bIsNativeBranchingPoint = true;
	
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = true;
#endif // WITH_EDITORONLY_DATA
}

void URsAnimNotifyState_Targeting::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Targets.Reset();
	PerformOverlapping(MeshComp, MeshComp->GetSocketTransform(SocketName));
	PerformFiltering(MeshComp->GetOwner());
	PerformSorting(MeshComp->GetOwner());

#if WITH_EDITOR
	SocketNames = MeshComp->GetAllSocketNames();
#endif // WITH_EDITOR
}

void URsAnimNotifyState_Targeting::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
}

bool URsAnimNotifyState_Targeting::PerformOverlapping(const USkeletalMeshComponent* MeshComp, FTransform SourceTransform)
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
	
	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		Targets.AddUnique(OverlapResult.GetActor());
	}
	
	if (Targets.Num() > 0)
	{
		bSuccess = true;
	}

#if WITH_EDITOR
	if (World->WorldType == EWorldType::PIE && (bShowDebugInPIE || RsTargetingGlobals::bShowDebugTargeting == true))
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

void URsAnimNotifyState_Targeting::PerformFiltering(const AActor* Owner)
{
	for (int32 i = Targets.Num() - 1; i >= 0; --i)
	{
		if (AActor* Target = Targets[i].Get())
		{
			const bool bSelfCheck = Target == Owner;
			const bool bSameTeam = URsAILibrary::GetTeamID(Target) == URsAILibrary::GetTeamID(Owner);
			
			if (!bIncludeSelf && bSelfCheck)
			{
				Targets.RemoveAt(i);
				continue;
			}
			
			if (!bIncludeFriendlyTeam && bSameTeam && !bSelfCheck)
			{
				Targets.RemoveAt(i);
				continue;
			}
			
			if (!bIncludeHostileTeam && !bSameTeam && !bSelfCheck)
			{
				Targets.RemoveAt(i);
				continue;
			}
			
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Target))
			{
				FGameplayTagContainer OutTags;
				TagInterface->GetOwnedGameplayTags(OutTags);
				if (TargetRequirements.RequirementsMet(OutTags) == false)
				{
					Targets.RemoveAt(i);
					continue;
				}
			}
		}
	}

	if ((MaxTargetCount > 0) && (Targets.Num() > MaxTargetCount))
	{
		Targets.SetNum(MaxTargetCount);
	}
}

void URsAnimNotifyState_Targeting::PerformSorting(const AActor* Owner)
{
	/** Sorting */
	if (bSortByDistance == true)
	{
		Targets.Sort([&Owner](const TWeakObjectPtr<AActor>& A, const TWeakObjectPtr<AActor>& B)
		{
			AActor* TargetA = A.Get();
			AActor* TargetB = B.Get();
			if (!IsValid(TargetA))
			{
				return false;
			}
			if (!IsValid(TargetB))
			{
				return true;
			}
			return FVector::DistSquared(TargetA->GetActorLocation(), Owner->GetActorLocation()) < FVector::DistSquared(TargetB->GetActorLocation(), Owner->GetActorLocation());
		});
	}
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