// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ERsCameraRig : uint8
{
	ThirdPerson,
	ShoulderView,
	LockOn,
	LinkSkill,
};

UENUM(BlueprintType)
enum class ERsCharacterFacingMode : uint8
{
	MovementDirection,
	CameraDirection,
};