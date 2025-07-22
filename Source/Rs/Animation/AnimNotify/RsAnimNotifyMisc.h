// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ERsPositionMode : uint8
{
	TowardTarget,
	LocalPosition_Source,
	LocalPosition_Target,
	WorldPosition,
};