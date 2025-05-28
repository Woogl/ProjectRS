// Copyright 2024 Team BH.


#include "RsTargetingTypes.h"

FCollisionShape FRsTargetingCollision::MakeShape() const
{
	switch (ShapeType)
	{
	case ERsTargetingShape::Box:
		return FCollisionShape::MakeBox(HalfExtent);

	case ERsTargetingShape::Sphere:
		return FCollisionShape::MakeSphere(FMath::Min3(HalfExtent.X, HalfExtent.Y, HalfExtent.Z));

	case ERsTargetingShape::Capsule:
		return FCollisionShape::MakeCapsule(FMath::Min(HalfExtent.X, HalfExtent.Y), HalfExtent.Z);
	}
	
	return FCollisionShape();
}

FRsTargetingCollision::FRsTargetingCollision(TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes, ERsTargetingShape ShapeType, FVector HalfExtent)
: CollisionObjectTypes(CollisionObjectTypes), ShapeType(ShapeType), HalfExtent(HalfExtent)
{
}

FRsTargetingFilter::FRsTargetingFilter(bool bIncludeSelf, bool bIncludeFriendlyTeam, bool bIncludeHostileTeam, int32 MaxTargetCount, FGameplayTagRequirements TargetRequirements, TArray<AActor*> ActorsToIgnore)
: bIncludeSelf(bIncludeSelf), bIncludeFriendlyTeam(bIncludeFriendlyTeam), bIncludeHostileTeam(bIncludeHostileTeam), MaxTargetCount(MaxTargetCount), TargetRequirements(TargetRequirements), ActorsToIgnore(ActorsToIgnore)
{
}

FRsTargetingSorter::FRsTargetingSorter(ERsSortingOrder SortByDistance)
: SortByDistance(SortByDistance)
{
}
