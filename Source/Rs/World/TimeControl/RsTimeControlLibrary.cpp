// Copyright 2025 Team BH.


#include "RsTimeControlLibrary.h"

#include "RsTimeControlSubsystem.h"

void URsTimeControlLibrary::RequestTimePause(UObject* WorldContext, FName Key, ERsTimeControlPriority Priority, float BlendTime)
{
	if (UWorld* World = WorldContext->GetWorld())
	{
		if (URsTimeControlSubsystem* TimeControlSubsystem = World->GetSubsystem<URsTimeControlSubsystem>())
		{
			TimeControlSubsystem->RequestTimePause(Key, Priority, BlendTime);
		}
	}
}

void URsTimeControlLibrary::RequestTimeDilation(UObject* WorldContext, FName Key, ERsTimeControlPriority Priority, float Dilation, float Duration, float BlendTime)
{
	if (UWorld* World = WorldContext->GetWorld())
	{
		if (URsTimeControlSubsystem* TimeControlSubsystem = World->GetSubsystem<URsTimeControlSubsystem>())
		{
			TimeControlSubsystem->RequestTimeDilation(Key, Priority, Dilation, Duration, BlendTime);
		}
	}
}

void URsTimeControlLibrary::RequestTimeResume(UObject* WorldContext, FName Key, float BlendTime)
{
	if (UWorld* World = WorldContext->GetWorld())
	{
		if (URsTimeControlSubsystem* TimeControlSubsystem = World->GetSubsystem<URsTimeControlSubsystem>())
		{
			TimeControlSubsystem->RequestTimeResume(Key, BlendTime);
		}
	}
}
