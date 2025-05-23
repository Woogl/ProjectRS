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