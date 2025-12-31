// Copyright 2024 Team BH.


#include "RsTargetingLibrary.h"

#include "GenericTeamAgentInterface.h"
#include "RsTargetingInterface.h"
#include "RsTargetingSettings.h"
#include "Engine/OverlapResult.h"
#include "Rs/Condition/RsCondition.h"
#include "TargetingSystem/TargetingSubsystem.h"

namespace RsTargetingGlobals
{
	bool bShowDebug = false;
	float DebugTime = 0.5f;
	
	static FAutoConsoleVariableRef CVarTargetingShowDebug(TEXT("rs.Targeting.ShowDebug"), bShowDebug, TEXT("Enable/Disable targeting collision debug shapes during gameplay.  ex) rs.Targeting.ShowDebug [1/0]"), ECVF_Cheat);
	static FAutoConsoleVariableRef CVarTargetingDebugTime(TEXT("rs.Targeting.DebugTime"), DebugTime, TEXT("Set the duration of the debug shapes for targeting.  ex) rs.Targeting.DebugTime [Sec]"), ECVF_Cheat);
}

bool URsTargetingLibrary::PerformTargeting(const AActor* Owner, FTransform Transform, const FRsTargetingParams& Params, TArray<AActor*>& ResultActors)
{
	TArray<AActor*> OverlappedActors = PerformOverlapping(Owner, Transform, Params.Shape);
	TArray<AActor*> FilteredActors = PerformFiltering(OverlappedActors, Owner, Params.Filter);
	TArray<AActor*> SortedActors = PerformSorting(FilteredActors, Owner, Params.Sorter);
	ResultActors = MoveTemp(SortedActors);
	
	bool bSuccess = ResultActors.Num() > 0;
	if (UWorld* World = Owner->GetWorld())
	{
		DrawDebugShape(World, Transform, Params.Shape, bSuccess);
	}
	return bSuccess;
}

bool URsTargetingLibrary::PerformTargetingFromComponent(const USceneComponent* Comp, const FRsTargetingParams& Params, TArray<AActor*>& ResultActors)
{
	const FTransform QueryTransform = Params.Shape.Offset * Comp->GetComponentTransform();
	return PerformTargeting(Comp->GetOwner(), QueryTransform, Params, ResultActors);
}

bool URsTargetingLibrary::PerformTargetingSwept(const AActor* Owner, FTransform Start, FTransform End, const FRsTargetingParams& Params, TArray<AActor*>& ResultActors)
{
	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return false;
	}
	
	FVector StartLoc = Start.GetLocation();
	FVector EndLoc = End.GetLocation();
	if (StartLoc == EndLoc)
	{
		PerformTargeting(Owner, Start, Params, ResultActors);
		return ResultActors.Num() > 0;
	}
   
	// Calculate substep num based on distance.
	float Dist = FVector::Dist(StartLoc, EndLoc);
	FVector Dir = (EndLoc - StartLoc).GetSafeNormal();
	FVector Extent = Params.Shape.MakeShape().GetExtent();
	float Thickness = (FMath::Abs(Dir.X) * Extent.X + FMath::Abs(Dir.Y) * Extent.Y + FMath::Abs(Dir.Z) * Extent.Z) * 2;
	int32 Steps = FMath::CeilToInt(Dist / Thickness);
   
	// Perform targeting from last transform to current transform without leaving any gaps.
	TSet<AActor*> OverlappedSet;
	for (int32 i = 0; i <= Steps; ++i)
	{
		float Alpha = static_cast<float>(i) / Steps;
		FTransform SubstepTransform;
		SubstepTransform.Blend(Start, End, Alpha);
		TArray<AActor*> OverlappedActors = PerformOverlapping(Owner, SubstepTransform, Params.Shape);
		OverlappedSet.Append(OverlappedActors);
		DrawDebugShape(World, SubstepTransform, Params.Shape, false);
	}

	TArray<AActor*> FilteredActors = PerformFiltering(OverlappedSet.Array(), Owner, Params.Filter);
	TArray<AActor*> SortedActors = PerformSorting(FilteredActors, Owner, Params.Sorter);
	ResultActors = MoveTemp(SortedActors);
   
	bool bSuccess = ResultActors.Num() > 0;
	if (bSuccess)
	{
		DrawDebugShape(World, Start, Params.Shape, true);
	}
	return bSuccess;
}

TArray<AActor*> URsTargetingLibrary::PerformOverlapping(const UObject* WorldContext, FTransform Transform, const FRsTargetingShape& Shape)
{
	TArray<AActor*> ResultActors;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return ResultActors;
	}

	TArray<FOverlapResult> OverlapResults;
	World->OverlapMultiByProfile(OverlapResults, Transform.GetLocation(), Transform.GetRotation(), TEXT("OverlapAll"), Shape.MakeShape());

	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		if (AActor* Actor = OverlapResult.GetActor())
		{
			ResultActors.AddUnique(Actor);
		}
	}
	
	return ResultActors;
}

TArray<AActor*> URsTargetingLibrary::PerformFiltering(const TArray<AActor*>& InActors, const AActor* Owner, const FRsTargetingFilter& Filter)
{
	TArray<AActor*> FilteredResult = InActors;

	FGenericTeamId OwnerTeamId = FGenericTeamId::NoTeam;
	if (const IGenericTeamAgentInterface* OwnerTeamInterface = Cast<IGenericTeamAgentInterface>(Owner))
	{
		OwnerTeamId = OwnerTeamInterface->GetGenericTeamId();
	}

	for (int32 i = FilteredResult.Num() - 1; i >= 0; --i)
	{
		AActor* Target = FilteredResult[i];
		if (!Target)
		{
			FilteredResult.RemoveAt(i);
			continue;
		}

		if (IRsTargetingInterface* TargetingInterface = Cast<IRsTargetingInterface>(Target))
		{
			if (!TargetingInterface->Execute_IsTargetable(Target))
			{
				FilteredResult.RemoveAt(i);
				continue;
			}
		}
		else
		{
			FilteredResult.RemoveAt(i);
			continue;
		}

		FGenericTeamId TargetTeamId = FGenericTeamId::NoTeam;
		if (const IGenericTeamAgentInterface* TargetTeamInterface = Cast<IGenericTeamAgentInterface>(Target))
		{
			TargetTeamId = TargetTeamInterface->GetGenericTeamId();
		}

		const bool bSelfCheck = (Target == Owner);
		const bool bSameTeam = (OwnerTeamId == TargetTeamId);

		if (!Filter.bIncludeSelf && bSelfCheck)
		{
			FilteredResult.RemoveAt(i);
			continue;
		}

		if (!Filter.bIncludeFriendlyTeam && bSameTeam && !bSelfCheck)
		{
			FilteredResult.RemoveAt(i);
			continue;
		}

		if (!Filter.bIncludeHostileTeam && !bSameTeam && !bSelfCheck)
		{
			FilteredResult.RemoveAt(i);
			continue;
		}

		if (Filter.Condition && !Filter.Condition->IsSatisfied(Target))
		{
			FilteredResult.RemoveAt(i);
			continue;
		}

		if (Filter.ActorsToIgnore.Contains(Target))
		{
			FilteredResult.RemoveAt(i);
			continue;
		}
	}

	return FilteredResult;
}

TArray<AActor*> URsTargetingLibrary::PerformSorting(const TArray<AActor*>& InActors, const AActor* Owner, const FRsTargetingSorter& Sorter)
{
	TArray<AActor*> SortedResult = InActors;

	if (Sorter.ByDistance != ERsSortingOrder::None && Owner)
	{
		const FVector OwnerLocation = Owner->GetActorLocation();
		const ERsSortingOrder Order = Sorter.ByDistance;
		SortedResult.Sort([OwnerLocation, Order](const AActor& A, const AActor& B)
		{
			if (Order == ERsSortingOrder::Ascending)
			{
				return FVector::DistSquared(A.GetActorLocation(), OwnerLocation) < FVector::DistSquared(B.GetActorLocation(), OwnerLocation);
			}
			if (Order == ERsSortingOrder::Descending)
			{
				return FVector::DistSquared(A.GetActorLocation(), OwnerLocation) > FVector::DistSquared(B.GetActorLocation(), OwnerLocation);
			}
			return false;
		});
	}
	
	return SortedResult;
}

bool URsTargetingLibrary::ExecuteTargetingPreset(AActor* SourceActor, const UTargetingPreset* TargetingPreset, TArray<AActor*>& ResultActors)
{
	if (!SourceActor || !TargetingPreset)
	{
		return false;
	}
	
	UTargetingSubsystem* TargetingSubsystem = UTargetingSubsystem::Get(SourceActor->GetWorld());
	if (TargetingSubsystem == nullptr)
	{
		return false;
	}
	
	FTargetingSourceContext Context;
	Context.SourceActor = SourceActor;
	Context.InstigatorActor = SourceActor;
	Context.SourceObject = SourceActor;
	Context.SourceLocation = SourceActor->GetActorLocation();
	
	FTargetingRequestHandle Handle = TargetingSubsystem->MakeTargetRequestHandle(TargetingPreset, Context);
	TargetingSubsystem->ExecuteTargetingRequestWithHandle(Handle);
	
	TArray<FHitResult> HitResults;
	TargetingSubsystem->GetTargetingResults(Handle, HitResults);
	
	for (const FHitResult& HitResult : HitResults)
	{
		ResultActors.Add(HitResult.GetActor());
	}

	TargetingSubsystem->ReleaseTargetRequestHandle(Handle);
	
	return !ResultActors.IsEmpty();
}

void URsTargetingLibrary::DrawDebugShape(const UWorld* World, const FTransform& Transform, const FRsTargetingShape& Shape, bool bSuccess)
{
	if (!World)
	{
		return;
	}
	if (!ShouldDrawDebug(World))
	{
		return;
	}

	const URsTargetingSettings* Settings = &URsTargetingSettings::Get();
	if (!Settings)
	{
		return;
	}
	FColor Color = bSuccess ? Settings->SuccessColor : Settings->FailureColor;
	
	bool bPersistentLines = false;
	uint8 DepthPriority = 0;
	float Thickness = 1.f;

	FCollisionShape CollisionShape = Shape.MakeShape();
	switch (Shape.ShapeType)
	{
	case ERsTargetingShapeType::Box:
		DrawDebugBox(World, Transform.GetLocation(), CollisionShape.GetExtent(), Transform.GetRotation(), Color, bPersistentLines, RsTargetingGlobals::DebugTime, DepthPriority, Thickness);
		break;
		
	case ERsTargetingShapeType::Sphere:
		DrawDebugCapsule(World, Transform.GetLocation(), CollisionShape.GetSphereRadius(), CollisionShape.GetSphereRadius(), Transform.GetRotation(), Color, bPersistentLines, RsTargetingGlobals::DebugTime, DepthPriority, Thickness);
		break;
		
	case ERsTargetingShapeType::Capsule:
		DrawDebugCapsule(World, Transform.GetLocation(), CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), Transform.GetRotation(), Color, bPersistentLines, RsTargetingGlobals::DebugTime, DepthPriority, Thickness);
		break;
	}
}

void URsTargetingLibrary::DrawDebugArrow(const UWorld* World, const FVector& Start, const FVector& End, FColor Color)
{
	if (!World)
	{
		return;
	}
	if (!ShouldDrawDebug(World))
	{
		return;
	}
	
	DrawDebugDirectionalArrow(World, Start, End, 2.f, Color, false, RsTargetingGlobals::DebugTime, 0, 1.f);
}

bool URsTargetingLibrary::ShouldDrawDebug(const UWorld* World)
{
	if (!World)
	{
		return false;
	}
	if (World->WorldType == EWorldType::EditorPreview)
	{
		return true;
	}
	if (World->WorldType == EWorldType::PIE && RsTargetingGlobals::bShowDebug)
	{
		return true;
	}
	return false;
}
