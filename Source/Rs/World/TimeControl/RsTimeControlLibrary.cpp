// Copyright 2025 Team BH.


#include "RsTimeControlLibrary.h"

#include "RsTimeControlSubsystem.h"

void URsTimeControlLibrary::RequestTimePause(UObject* WorldContext, FName RequestKey, ERsTimeControlPriority Priority)
{
	if (UWorld* World = WorldContext->GetWorld())
	{
		if (URsTimeControlSubsystem* TimeControlSubsystem = World->GetSubsystem<URsTimeControlSubsystem>())
		{
			TimeControlSubsystem->RequestTimePause(RequestKey, Priority);
		}
	}
}

void URsTimeControlLibrary::RequestTimeDilation(UObject* WorldContext, FName RequestKey, ERsTimeControlPriority Priority, float Dilation, float Duration)
{
	if (UWorld* World = WorldContext->GetWorld())
	{
		if (URsTimeControlSubsystem* TimeControlSubsystem = World->GetSubsystem<URsTimeControlSubsystem>())
		{
			TimeControlSubsystem->RequestTimeDilation(RequestKey, Priority, Dilation, Duration);
		}
	}
}

void URsTimeControlLibrary::RequestTimeResume(UObject* WorldContext, FName RequestKey)
{
	if (UWorld* World = WorldContext->GetWorld())
	{
		if (URsTimeControlSubsystem* TimeControlSubsystem = World->GetSubsystem<URsTimeControlSubsystem>())
		{
			TimeControlSubsystem->RequestTimeResume(RequestKey);
		}
	}
}
