// Copyright 2024 Team BH.


#include "RsTargetingTypes.h"

namespace RsTargetingGlobals
{
	bool GShowDebugTargeting = false;
	
	inline FAutoConsoleVariableRef CVarShowDebugTargeting(TEXT("rs.Targeting.ShowDebug"), GShowDebugTargeting, TEXT("Show targeting shape in gameplay."), ECVF_Cheat);
}