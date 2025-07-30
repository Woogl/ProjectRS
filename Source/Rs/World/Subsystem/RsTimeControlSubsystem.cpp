// Copyright 2025 Team BH.


#include "RsTimeControlSubsystem.h"

#include "Kismet/GameplayStatics.h"

float FTimeControlRequest::GetEndTime() const
{
	return StartTime + Duration;
}

bool FTimeControlRequest::IsEnd(float CurrentTime) const
{
	if (Duration < 0.f)
	{
		return false;
	}
	return CurrentTime >= GetEndTime();
}

TStatId URsTimeControlSubsystem::GetStatId() const
{
	return TStatId();
}

void URsTimeControlSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentTime = GetWorld()->GetTimeSeconds();
	float CurrentDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());

	// Remove expired requests.
	for (int32 i = Requests.Num() - 1; i >= 0; --i)
	{
		if (Requests[i].IsEnd(CurrentTime))
		{
			Requests.RemoveAt(i);
		}
	}

	// Restore default time dilation if no active requests remain.
	if (Requests.IsEmpty())
	{
		if (!FMath::IsNearlyEqual(CurrentDilation, 1.f))
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
		}
		return;
	}

	// Select the highest-priority request. If priorities same, select the latest one.
	const FTimeControlRequest* RequestToApply = &Requests[0];
	for (int32 i = 1; i < Requests.Num(); ++i)
	{
		const FTimeControlRequest& Request = Requests[i];
		if (Request.Priority < RequestToApply->Priority || (Request.Priority == RequestToApply->Priority && Request.StartTime > RequestToApply->StartTime))
		{
			RequestToApply = &Request;
		}
	}

	// Apply new time dilation.
	if (!FMath::IsNearlyEqual(CurrentDilation, RequestToApply->Dilation))
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), RequestToApply->Dilation);
	}
}

void URsTimeControlSubsystem::RequestTimePause(ERsTimeControlPriority Priority)
{
	RequestTimeDilation(Priority, 0.f, -1.f);
}

void URsTimeControlSubsystem::RequestTimeResume(ERsTimeControlPriority Priority)
{
	for (int32 i = Requests.Num() - 1; i >= 0; --i)
	{
		if (Requests[i].Priority >= Priority)
		{
			Requests.RemoveAt(i);
		}
	}
}

void URsTimeControlSubsystem::RequestTimeDilation(ERsTimeControlPriority Priority, float Dilation, float Duration)
{
	FTimeControlRequest Request;
	Request.Priority = Priority;
	Request.Dilation = Dilation;
	Request.StartTime = GetWorld()->GetTimeSeconds();
	Request.Duration = Duration;
	
	Requests.Add(Request);
}
