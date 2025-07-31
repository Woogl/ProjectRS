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

	ERsTimeControlPriority Priority = ERsTimeControlPriority::VFX;
	float Dilation = 0.f;
	float StartTime = 0.f;
	float Duration = 0.f;

	float GetEndTime() const;
	bool IsEnd(float CurrentTime) const;
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
	
	void RequestTimePause(FName RequestKey, ERsTimeControlPriority Priority);
	void RequestTimeDilation(FName RequestKey, ERsTimeControlPriority Priority, float Dilation, float Duration);
	void RequestTimeResume(FName RequestKey);
	void ClearRequests();

private:
	UPROPERTY()
	TMap<FName, FTimeControlRequest> Requests;
};
