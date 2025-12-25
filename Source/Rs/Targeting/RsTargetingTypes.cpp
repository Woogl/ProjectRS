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

FRsTargetingParams::FRsTargetingParams(FRsTargetingShape Shape, FRsTargetingCollision Collision, FRsTargetingFilter Filter, FRsTargetingSorter Sorter)
	: Shape(Shape), Collision(Collision), Filter(Filter), Sorter(Sorter)
{
}
