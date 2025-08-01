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

float FTimeControlRequest::GetDilation(float CurrentTime) const
{
	if (CurrentTime < StartTime)
	{
		return 1.f;
	}
	else if (BlendTime <= 0.f)
	{
		return TargetDilation;
	}
	float Alpha = FMath::Clamp((CurrentTime - StartTime) / BlendTime, 0.f, 1.f);
	return FMath::Lerp(1.f, TargetDilation, Alpha);
}

void URsTimeControlSubsystem::Deinitialize()
{
	ClearRequests();
	
	Super::Deinitialize();
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
		const FTimeControlRequest& Request = It.Value();
		if (Request.IsEnd(CurrentTime))
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

	const FTimeControlRequest* RequestToApply = nullptr;
	for (const auto& [Key, Request] : Requests)
	{
		if (RequestToApply == nullptr)
		{
			RequestToApply = &Request;
		}
		// Select the highest-priority request.
		else if (Request.Priority > RequestToApply->Priority)
		{
			RequestToApply = &Request;
		}
		// If priorities same, select the latest one.
		else if (Request.Priority == RequestToApply->Priority && Request.StartTime > RequestToApply->StartTime)
		{
			RequestToApply = &Request;
		}
	}

	// Apply new time dilation.
	float NewDilation = RequestToApply->GetDilation(CurrentTime);
	if (!FMath::IsNearlyEqual(CurrentDilation, NewDilation))
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), NewDilation);
	}
}

void URsTimeControlSubsystem::RequestTimePause(FName RequestKey, ERsTimeControlPriority Priority, float BlendTime)
{
	RequestTimeDilation(RequestKey, Priority, 0.f, -1.f, BlendTime);
}

void URsTimeControlSubsystem::RequestTimeDilation(FName RequestKey, ERsTimeControlPriority Priority, float Dilation, float Duration, float BlendTime)
{
	FTimeControlRequest Request;
	Request.Priority = Priority;
	Request.TargetDilation = Dilation;
	Request.StartTime = GetWorld()->GetTimeSeconds();
	Request.Duration = Duration;
	Request.BlendTime = BlendTime;
	
	Requests.Add(RequestKey, Request);
}

void URsTimeControlSubsystem::RequestTimeResume(FName RequestKey, float BlendTime)
{
	FTimeControlRequest ResumeRequest;
	ResumeRequest.StartTime = GetWorld()->GetTimeSeconds();
	ResumeRequest.Duration = BlendTime;
	ResumeRequest.BlendTime = BlendTime;
	ResumeRequest.TargetDilation = 1.f;
	if (FTimeControlRequest* Request = Requests.Find(RequestKey))
	{
		ResumeRequest.Priority = Request->Priority;
	}
	else
	{
		ResumeRequest.Priority = ERsTimeControlPriority::None;
	}

	Requests.Add(RequestKey, ResumeRequest);
}

void URsTimeControlSubsystem::ClearRequests()
{
	Requests.Empty();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}
