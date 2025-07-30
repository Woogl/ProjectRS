// Copyright 2025 Team BH.


#include "RsWorldTimeLibrary.h"

#include "Subsystem/RsTimeControlSubsystem.h"

void URsWorldTimeLibrary::RequestTimePause(UObject* WorldContext, ERsTimeControlPriority Priority)
{
	if (UWorld* World = WorldContext->GetWorld())
	{
		if (URsTimeControlSubsystem* TimeControlSubsystem = World->GetSubsystem<URsTimeControlSubsystem>())
		{
			TimeControlSubsystem->RequestTimePause(Priority);
		}
	}
}

void URsWorldTimeLibrary::RequestTimeResume(UObject* WorldContext, ERsTimeControlPriority Priority)
{
	if (UWorld* World = WorldContext->GetWorld())
	{
		if (URsTimeControlSubsystem* TimeControlSubsystem = World->GetSubsystem<URsTimeControlSubsystem>())
		{
			TimeControlSubsystem->RequestTimeResume(Priority);
		}
	}
}

void URsWorldTimeLibrary::RequestTimeDilation(UObject* WorldContext, ERsTimeControlPriority Priority, float Dilation, float Duration)
{
	if (UWorld* World = WorldContext->GetWorld())
	{
		if (URsTimeControlSubsystem* TimeControlSubsystem = World->GetSubsystem<URsTimeControlSubsystem>())
		{
			TimeControlSubsystem->RequestTimeDilation(Priority, Dilation, Duration);
		}
	}
}
