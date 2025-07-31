// Copyright 2024 Team BH.


#include "RsTargetingTypes.h"


FCollisionShape FRsTargetingShape::MakeShape() const
{
	switch (ShapeType)
	{
	case ERsTargetingShapeType::Box:
		return FCollisionShape::MakeBox(HalfExtent);

	case ERsTargetingShapeType::Sphere:
		return FCollisionShape::MakeSphere(FMath::Min3(HalfExtent.X, HalfExtent.Y, HalfExtent.Z));

	case ERsTargetingShapeType::Capsule:
		return FCollisionShape::MakeCapsule(FMath::Min(HalfExtent.X, HalfExtent.Y), HalfExtent.Z);
	}
	
	return FCollisionShape();
}

FRsTargetingShape::FRsTargetingShape(ERsTargetingShapeType ShapeType, FVector HalfExtent)
	: ShapeType(ShapeType), HalfExtent(HalfExtent)
{
}

FRsTargetingFilter::FRsTargetingFilter(bool bIncludeSelf, bool bIncludeFriendlyTeam, bool bIncludeHostileTeam, FGameplayTagRequirements TagRequirements, TArray<AActor*> ActorsToIgnore)
	: bIncludeSelf(bIncludeSelf), bIncludeFriendlyTeam(bIncludeFriendlyTeam), bIncludeHostileTeam(bIncludeHostileTeam), TagRequirements(TagRequirements), ActorsToIgnore(ActorsToIgnore)
{
}

FRsTargetingSorter::FRsTargetingSorter(ERsSortingOrder ByDistance, ERsSortingOrder ByTag, FGameplayTag SortingTag)
	: ByDistance(ByDistance), ByTag(ByTag), SortingTag(SortingTag)
{
}

FRsTargetingParams::FRsTargetingParams(FRsTargetingShape Shape, FRsTargetingCollision Collision, FRsTargetingFilter Filter, FRsTargetingSorter Sorter)
	: Shape(Shape), Collision(Collision), Filter(Filter), Sorter(Sorter)
{
}
