// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM()
enum class ERsEffectTarget : uint8
{
	Source	UMETA(ToolTip="Apply GE to self"),
	Target	UMETA(ToolTip="Apply GE to target"),
};
