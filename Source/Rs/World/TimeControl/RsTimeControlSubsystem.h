// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsTimeControlLibrary.h"
#include "Subsystems/WorldSubsystem.h"
#include "RsTimeControlSubsystem.generated.h"

USTRUCT()
struct FTimeControlRequest
{
	GENERATED_BODY()
	
	float StartTime = 0.f;
	float Duration = 0.f;
	float TargetDilation = 0.f;
	float BlendTime = 0.f;
	ERsTimeControlPriority Priority = ERsTimeControlPriority::None;
	
	float GetEndTime() const;
	bool IsEnd(float CurrentTime) const;
	float GetDilation(float CurrentTime) const;
};

/**
 * 
 */
UCLASS()
class RS_API URsTimeControlSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;
	
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	
	void RequestTimePause(FName RequestKey, ERsTimeControlPriority Priority, float BlendTime);
	void RequestTimeDilation(FName RequestKey, ERsTimeControlPriority Priority, float Dilation, float Duration, float BlendTime);
	void RequestTimeResume(FName RequestKey, float BlendTime);
	void ClearRequests();

private:
	UPROPERTY()
	TMap<FName, FTimeControlRequest> Requests;
};
