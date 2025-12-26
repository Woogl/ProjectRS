// Copyright 2024 Team BH.


#include "RsTargetingLibrary.h"

#include "GenericTeamAgentInterface.h"
#include "RsTargetingInterface.h"
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

bool URsTargetingLibrary::PerformTargeting(const AActor* Owner, FTransform Transform, const FRsTargetingParams& Params, TArray<AActor*>& ResultActors, bool bDrawDebug)
{
	TArray<AActor*> OverlappedActors = PerformOverlapping(Owner, Transform, Params.Shape, Params.Collision, false);
	TArray<AActor*> FilteredActors = PerformFiltering(OverlappedActors, Owner, Params.Filter);
	TArray<AActor*> SortedActors = PerformSorting(FilteredActors, Owner, Params.Sorter);
	ResultActors = SortedActors;
	bool bSuccess = ResultActors.Num() > 0;

	if (UWorld* World = Owner->GetWorld())
	{
		FColor Color = bSuccess ? FColor::Green : FColor::Red;
		DrawDebugShape(World, Transform, Params.Shape, Params.Collision, Color);
	}
	
	return bSuccess;
}

bool URsTargetingLibrary::PerformTargetingWithSubsteps(const AActor* Owner, FTransform Start, FTransform End, int32 MaxSubsteps, const FRsTargetingParams& Params, TArray<AActor*>& ResultActors, bool bDrawDebug)
{
	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return false;
	}
	
	FVector StartLoc = Start.GetLocation();
	FVector EndLoc = End.GetLocation();
	float DeltaDistance = FVector::Dist(StartLoc, EndLoc);

	// Calculate substep num based on distance.
	FVector ShapeExtent = Params.Shape.MakeShape().GetExtent();
	int32 SubstepNum = FMath::CeilToInt(DeltaDistance / FMath::Min3(ShapeExtent.X, ShapeExtent.Y, ShapeExtent.Z));
	SubstepNum = FMath::Min(SubstepNum, MaxSubsteps);
	
	// Perform targeting from last transform to current transform without leaving any gaps.
	TSet<AActor*> OverlappedSet;
	for (int32 i = 0; i <= SubstepNum; ++i)
	{
		float Alpha = static_cast<float>(i) / SubstepNum;
		FTransform SubstepTransform;
		SubstepTransform.Blend(Start, End, Alpha);
		TArray<AActor*> SubstepOverlappedActors = PerformOverlapping(Owner, SubstepTransform, Params.Shape, Params.Collision);
		OverlappedSet.Append(SubstepOverlappedActors);
		DrawDebugShape(World, SubstepTransform, Params.Shape, Params.Collision, FColor::Red);
	}

	TArray<AActor*> FilteredActors = PerformFiltering(OverlappedSet.Array(), Owner, Params.Filter);
	TArray<AActor*> SortedActors = PerformSorting(FilteredActors, Owner, Params.Sorter);
	ResultActors = SortedActors;
	bool bSuccess = ResultActors.Num() > 0;

	if (bSuccess)
	{
		DrawDebugShape(World, Start, Params.Shape, Params.Collision, FColor::Green);
	}
	
	return bSuccess;
}

TArray<AActor*> URsTargetingLibrary::PerformOverlapping(const UObject* WorldContext, FTransform Transform, const FRsTargetingShape& Shape, const FRsTargetingCollision& Collision, bool bDrawDebug)
{
	TArray<AActor*> ResultActors;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return ResultActors;
	}

	TArray<FOverlapResult> OverlapResults;

	// By collision object types
	FCollisionObjectQueryParams ObjectParams;
	for (auto Iter = Collision.CollisionObjectTypes.CreateConstIterator(); Iter; ++Iter)
	{
		const ECollisionChannel Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
		ObjectParams.AddObjectTypesToQuery(Channel);
	}
	if (ObjectParams.IsValid())
	{
		World->OverlapMultiByObjectType(OverlapResults, Transform.GetLocation(), Transform.GetRotation(), ObjectParams, Shape.MakeShape());
	}

	// By collision channels
	for (TEnumAsByte<ECollisionChannel> Channel : Collision.CollisionChannels)
	{
		TArray<FOverlapResult> ChannelResults;
		World->OverlapMultiByChannel(ChannelResults, Transform.GetLocation(), Transform.GetRotation(), Channel, Shape.MakeShape());
		OverlapResults.Append(ChannelResults);
	}

	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		if (AActor* Actor = OverlapResult.GetActor())
		{
			ResultActors.AddUnique(Actor);
		}
	}
	
	if (bDrawDebug)
	{
		FColor Color = ResultActors.IsEmpty() ? FColor::Red : FColor::Green;
		DrawDebugShape(World, Transform, Shape, Collision, Color);
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

void URsTargetingLibrary::DrawDebugShape(const UWorld* World, const FTransform& Transform, const FRsTargetingShape& Shape, const FRsTargetingCollision& Collision, FColor Color)
{
	if (!World)
	{
		return;
	}
	if (!ShouldDrawDebug(World))
	{
		return;
	}
	
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

FTransform URsTargetingLibrary::GetSocketWorldTransform(const USceneComponent* Component, FName SocketName, const FTransform& LocalOffset)
{
	if (!Component)
	{
		return FTransform();
	}
	
	FTransform ComponentTransform = Component->GetSocketTransform(SocketName, RTS_Component);
	FTransform WorldTransform = (ComponentTransform * LocalOffset) * Component->GetComponentTransform();
	return WorldTransform;
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
