// Copyright 2024 Team BH.


#include "RsTargetingLibrary.h"

#include "GameplayTagAssetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Engine/OverlapResult.h"
#include "TargetingSystem/TargetingSubsystem.h"

namespace RsTargetingGlobals
{
	bool bShowDebugTargeting = false;
	float DebugTime = 0.5f;
	
	FAutoConsoleVariableRef CVarTargetingShowDebug(TEXT("rs.Targeting.ShowDebug"), bShowDebugTargeting, TEXT("Enable/Disable targeting collision debug shapes during gameplay."), ECVF_Cheat);
	FAutoConsoleVariableRef CVarTargetingDebugTime(TEXT("rs.Targeting.DebugTime"), DebugTime, TEXT("Set lifetime of the debug shapes for targeting."), ECVF_Cheat);
}

bool URsTargetingLibrary::PerformTargeting(AActor* Owner, FVector StartLoc, FRotator StartRot, const FRsTargetingCollision& Collision, const FRsTargetingFilter& Filter, const FRsTargetingSorter& Sorter, TArray<AActor*>& ResultActors)
{
	TArray<AActor*> OverlappedActors = PerformOverlapping(Owner, StartLoc, StartRot, Collision);
	TArray<AActor*> FilteredActors = PerformFiltering(OverlappedActors, Owner, Filter);
	TArray<AActor*> SortedActors = PerformSorting(FilteredActors, Owner, Sorter);
	ResultActors = SortedActors;
	bool bSuccess = ResultActors.Num() > 0;

#if WITH_EDITOR
	if (UWorld* World = Owner->GetWorld())
	{
		FColor Color = bSuccess ? FColor::Green : FColor::Red;
		if (World->WorldType == EWorldType::PIE && RsTargetingGlobals::bShowDebugTargeting == true)
		{
			DrawDebugShape(World, StartLoc, StartRot, Collision, Color);
		}
		else if (World->WorldType == EWorldType::EditorPreview)
		{
			DrawDebugShape(World, StartLoc, StartRot, Collision, Color);
		}
	}
#endif // WITH_EDITOR
	
	return bSuccess;
}

TArray<AActor*> URsTargetingLibrary::PerformOverlapping(UObject* WorldContext, FVector StartLoc, FRotator StartRot, const FRsTargetingCollision& Collision, bool bDrawDebug)
{
	TArray<AActor*> ResultActors;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return ResultActors;
	}

	TArray<FOverlapResult> OverlapResults;
	if (Collision.CollisionObjectTypes.Num() > 0)
	{
		FCollisionObjectQueryParams ObjectParams;
		for (auto Iter = Collision.CollisionObjectTypes.CreateConstIterator(); Iter; ++Iter)
		{
			const ECollisionChannel& Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
			ObjectParams.AddObjectTypesToQuery(Channel);
		}
		World->OverlapMultiByObjectType(OverlapResults, StartLoc, StartRot.Quaternion(), ObjectParams, Collision.MakeShape());
	}
	
	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		ResultActors.AddUnique(OverlapResult.GetActor());
	}

#if WITH_EDITOR
	bool bSuccess = ResultActors.Num() > 0;
	if (bDrawDebug && bSuccess)
	{
		FColor Color = bSuccess ? FColor::Green : FColor::Red;
		DrawDebugShape(World, StartLoc, StartRot, Collision, Color);
	}
#endif // WITH_EDITOR
	
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
			
			const bool bSelfCheck = Target == Owner;
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
			
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Target))
			{
				FGameplayTagContainer OutTags;
				TagInterface->GetOwnedGameplayTags(OutTags);
				if (Filter.TargetRequirements.RequirementsMet(OutTags) == false)
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

	if ((Filter.MaxTargetCount > 0) && (FilteredResult.Num() > Filter.MaxTargetCount))
	{
		FilteredResult.SetNum(Filter.MaxTargetCount);
	}
	
	return FilteredResult;
}

TArray<AActor*> URsTargetingLibrary::PerformSorting(const TArray<AActor*>& InActors, AActor* Owner, const FRsTargetingSorter& Sorter)
{
	TArray<AActor*> SortedResult = InActors;

	if (Sorter.bSortByDistance && Owner)
	{
		const FVector OwnerLocation = Owner->GetActorLocation();
		SortedResult.Sort([OwnerLocation](const AActor& A, const AActor& B)
		{
			return FVector::DistSquared(A.GetActorLocation(), OwnerLocation) < FVector::DistSquared(B.GetActorLocation(), OwnerLocation);
		});
	}
	
	return SortedResult;
}

void URsTargetingLibrary::DrawDebugShape(UObject* WorldContext, FVector StartLoc, FRotator StartRot, const FRsTargetingCollision& Collision, FColor Color)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return;
	}
	
	bool bPersistentLines = false;
	uint8 DepthPriority = 0;
	float Thickness = 1.f;

	FCollisionShape CollisionShape = Collision.MakeShape();
	switch (Collision.ShapeType)
	{
	case ERsTargetingShape::Box:
		DrawDebugBox(World, StartLoc, CollisionShape.GetExtent(), StartRot.Quaternion(), Color, bPersistentLines, RsTargetingGlobals::DebugTime, DepthPriority, Thickness);
		break;
		
	case ERsTargetingShape::Sphere:
		DrawDebugCapsule(World, StartLoc, CollisionShape.GetSphereRadius(), CollisionShape.GetSphereRadius(), StartRot.Quaternion(), Color, bPersistentLines, RsTargetingGlobals::DebugTime, DepthPriority, Thickness);
		break;
		
	case ERsTargetingShape::Capsule:
		DrawDebugCapsule(World, StartLoc, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), StartRot.Quaternion(), Color, bPersistentLines, RsTargetingGlobals::DebugTime, DepthPriority, Thickness);
		break;
		
	case ERsTargetingShape::Cylinder:
		const FVector RotatedExtent = StartLoc * CollisionShape.GetExtent();
		DrawDebugCylinder(World, StartLoc - RotatedExtent, StartLoc + RotatedExtent, CollisionShape.GetExtent().X, 16, Color, bPersistentLines, RsTargetingGlobals::DebugTime, DepthPriority, Thickness);
		break;
	}
}

bool URsTargetingLibrary::ExecuteTargetingPreset(AActor* SourceActor, const UTargetingPreset* TargetingPreset, TArray<AActor*>& ResultActors)
{
	if (SourceActor == nullptr || TargetingPreset == nullptr)
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
