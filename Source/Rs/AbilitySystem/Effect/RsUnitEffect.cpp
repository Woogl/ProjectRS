// Copyright 2025 Team BH.


#include "RsUnitEffect.h"

URsUnitEffect_Instant::URsUnitEffect_Instant()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
}

URsUnitEffect_Buff::URsUnitEffect_Buff()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
}

URsUnitEffect_Periodic::URsUnitEffect_Periodic()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Period = 0.5f;
}
