﻿// Copyright 2024 Team BH.


#include "RsTargetingLibrary.h"

#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Engine/OverlapResult.h"
#include "TargetingSystem/TargetingSubsystem.h"

namespace RsTargetingGlobals
{
	bool bShowDebug = false;
	float DebugTime = 0.5f;
	
	static FAutoConsoleVariableRef CVarTargetingShowDebug(TEXT("rs.Targeting.ShowDebug"), bShowDebug, TEXT("Enable/Disable targeting collision debug shapes during gameplay.  ex) rs.Targeting.ShowDebug [1/0]"), ECVF_Cheat);
	static FAutoConsoleVariableRef CVarTargetingDebugTime(TEXT("rs.Targeting.DebugTime"), DebugTime, TEXT("Set the duration of the debug shapes for targeting.  ex) rs.Targeting.DebugTime [Sec]"), ECVF_Cheat);

	bool ShouldDrawDebug(UWorld* World, bool bDrawDebug)
	{
		if (!World) return false;
		if (bDrawDebug) return true;
		if (World->WorldType == EWorldType::EditorPreview) return true;
		if (World->WorldType == EWorldType::PIE && bShowDebug) return true;
		return false;
	}
}

bool URsTargetingLibrary::PerformTargeting(AActor* Owner, FTransform Transform, const FRsTargetingParams& Params, TArray<AActor*>& ResultActors, bool bDrawDebug)
{
	TArray<AActor*> OverlappedActors = PerformOverlapping(Owner, Transform, Params.Shape, Params.Collision);
	TArray<AActor*> FilteredActors = PerformFiltering(OverlappedActors, Owner, Params.Filter);
	TArray<AActor*> SortedActors = PerformSorting(FilteredActors, Owner, Params.Sorter);
	ResultActors = SortedActors;
	bool bSuccess = ResultActors.Num() > 0;

	if (UWorld* World = Owner->GetWorld())
	{
		if (RsTargetingGlobals::ShouldDrawDebug(World, bDrawDebug) == true)
		{
			FColor Color = bSuccess ? FColor::Green : FColor::Red;
			DrawDebugShape(World, Transform, Params.Shape, Params.Collision, Color);
		}
	}
	
	return bSuccess;
}

bool URsTargetingLibrary::PerformTargetingWithSubsteps(AActor* Owner, FTransform Start, FTransform End, int32 MaxSubsteps, const FRsTargetingParams& Params, TArray<AActor*>& ResultActors, bool bDrawDebug)
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
		if (RsTargetingGlobals::ShouldDrawDebug(World, bDrawDebug) == true)
		{
			DrawDebugShape(World, SubstepTransform, Params.Shape, Params.Collision, FColor::Red);
		}
	}

	TArray<AActor*> FilteredActors = PerformFiltering(OverlappedSet.Array(), Owner, Params.Filter);
	TArray<AActor*> SortedActors = PerformSorting(FilteredActors, Owner, Params.Sorter);
	ResultActors = SortedActors;
	bool bSuccess = ResultActors.Num() > 0;

	if (RsTargetingGlobals::ShouldDrawDebug(World, bDrawDebug) == true && bSuccess)
	{
		DrawDebugShape(World, Start, Params.Shape, Params.Collision, FColor::Green);
	}
	
	return bSuccess;
}

TArray<AActor*> URsTargetingLibrary::PerformOverlapping(UObject* WorldContext, FTransform Transform, const FRsTargetingShape& Shape, const FRsTargetingCollision& Collision)
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
	
	return ResultActors;
}

TArray<AActor*> URsTargetingLibrary::PerformFiltering(const TArray<AActor*>& InActors, AActor* Owner, const FRsTargetingFilter& Filter)
{
	TArray<AActor*> FilteredResult = InActors;

	FGenericTeamId OwnerTeamId = FGenericTeamId::NoTeam;
	if (const IGenericTeamAgentInterface* OwnerTeamInterface = Cast<IGenericTeamAgentInterface>(Owner))
	{
		OwnerTeamId = OwnerTeamInterface->GetGenericTeamId();
	}
	
	for (int32 i = InActors.Num() - 1; i >= 0; --i)
	{
		if (AActor* Target = InActors[i])
		{
			FGenericTeamId TargetTeamId = FGenericTeamId::NoTeam;
			if (const IGenericTeamAgentInterface* TargetTeamInterface = Cast<IGenericTeamAgentInterface>(Target))
			{
				TargetTeamId = TargetTeamInterface->GetGenericTeamId();
			}
			
			const bool bSameTeam = (OwnerTeamId == TargetTeamId);
			const bool bSelfCheck = Target == Owner;
			
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
			
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Target))
			{
				FGameplayTagContainer OutTags;
				TagInterface->GetOwnedGameplayTags(OutTags);
				if (Filter.TagRequirements.RequirementsMet(OutTags) == false)
				{
					FilteredResult.RemoveAt(i);
					continue;
				}
			}

			if (Filter.ActorsToIgnore.Contains(Target))
			{
				FilteredResult.RemoveAt(i);
				continue;
			}
		}
	}
	
	return FilteredResult;
}

TArray<AActor*> URsTargetingLibrary::PerformSorting(const TArray<AActor*>& InActors, AActor* Owner, const FRsTargetingSorter& Sorter)
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
			else if (Order == ERsSortingOrder::Descending)
			{
				return FVector::DistSquared(A.GetActorLocation(), OwnerLocation) > FVector::DistSquared(B.GetActorLocation(), OwnerLocation);
			}
			else
			{
				return false;
			}
		});
	}

	if (Sorter.ByTags != ERsSortingOrder::None && Sorter.RequiredTags.IsValid())
	{
		SortedResult.Sort([&Sorter](const AActor& A, const AActor& B)
		{
			const IGameplayTagAssetInterface* ATagInterface = Cast<IGameplayTagAssetInterface>(&A);
			const IGameplayTagAssetInterface* BTagInterface = Cast<IGameplayTagAssetInterface>(&B);
			bool AHasTag = ATagInterface && ATagInterface->HasAllMatchingGameplayTags(Sorter.RequiredTags);
			bool BHasTag = BTagInterface && BTagInterface->HasAllMatchingGameplayTags(Sorter.RequiredTags);
			if (Sorter.ByTags == ERsSortingOrder::Ascending)
			{
				return AHasTag < BHasTag;
			}
			else if (Sorter.ByTags == ERsSortingOrder::Descending)
			{
				return AHasTag > BHasTag;
			}
			else
			{
				return false;
			}
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

void URsTargetingLibrary::DrawDebugShape(UObject* WorldContext, FTransform Transform, const FRsTargetingShape& Shape, const FRsTargetingCollision& Collision, FColor Color)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
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
