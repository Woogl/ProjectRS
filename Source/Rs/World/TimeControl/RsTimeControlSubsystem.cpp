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
	for (auto It = Requests.CreateIterator(); It; ++It)
	{
		if (It.Value().IsEnd(CurrentTime))
		{
			It.RemoveCurrent();
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
	const FTimeControlRequest* RequestToApply = nullptr;
	for (const auto& [Key, Request] : Requests)
	{
		if (RequestToApply == nullptr || Request.Priority < RequestToApply->Priority || (Request.Priority == RequestToApply->Priority && Request.StartTime > RequestToApply->StartTime))
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

void URsTimeControlSubsystem::RequestTimePause(FName RequestKey, ERsTimeControlPriority Priority)
{
	RequestTimeDilation(RequestKey, Priority, 0.f, -1.f);
}

void URsTimeControlSubsystem::RequestTimeDilation(FName RequestKey, ERsTimeControlPriority Priority, float Dilation, float Duration)
{
	FTimeControlRequest Request;
	Request.Priority = Priority;
	Request.Dilation = Dilation;
	Request.StartTime = GetWorld()->GetTimeSeconds();
	Request.Duration = Duration;
	
	Requests.Add(RequestKey, Request);
}

void URsTimeControlSubsystem::RequestTimeResume(FName RequestKey)
{
	Requests.Remove(RequestKey);
}

void URsTimeControlSubsystem::ClearRequests()
{
	Requests.Empty();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}
