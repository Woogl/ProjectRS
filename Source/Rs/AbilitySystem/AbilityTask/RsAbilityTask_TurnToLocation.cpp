// Copyright 2024 Team BH.


#include "RsAbilityTask_TurnToLocation.h"

#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

URsAbilityTask_TurnToLocation::URsAbilityTask_TurnToLocation()
{
	bTickingTask = true;
	bSimulatedTask = true;

	Duration = 0.f;
	RotatingSpeed = 0.f;

	TargetLocation = FVector::ZeroVector;
	StartTime = 0.f;
	EndTime = 0.f;

	bRotatePitch = false;
	bRotateYaw = true;
	bRotateRoll = false;
}

void URsAbilityTask_TurnToLocation::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, TargetLocation);
	DOREPLIFETIME(ThisClass, Duration);
	DOREPLIFETIME(ThisClass, RotatingSpeed);
}

void URsAbilityTask_TurnToLocation::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);

	StartTime = GetWorld()->GetTimeSeconds();
	EndTime = StartTime + Duration;
}

URsAbilityTask_TurnToLocation* URsAbilityTask_TurnToLocation::TurnToLocation(UGameplayAbility* OwningAbility, FVector TargetLocation, float Duration, float RotatingSpeed, bool bRotatePitch, bool bRotateYaw, bool bRotateRoll)
{
	URsAbilityTask_TurnToLocation* MyTask = NewAbilityTask<URsAbilityTask_TurnToLocation>(OwningAbility);
	MyTask->TargetLocation = TargetLocation;
	MyTask->Duration = Duration;
	MyTask->RotatingSpeed = RotatingSpeed;
	MyTask->bRotatePitch = bRotatePitch;
	MyTask->bRotateYaw = bRotateYaw;
	MyTask->bRotateRoll = bRotateRoll;
	return MyTask;
}

void URsAbilityTask_TurnToLocation::Activate()
{
	Super::Activate();

	StartTime = GetWorld()->GetTimeSeconds();
	EndTime = StartTime + Duration;
}

void URsAbilityTask_TurnToLocation::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	FRotator CurrentRotation = GetAvatarActor()->GetActorRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetAvatarActor()->GetActorLocation(), TargetLocation);

	float PitchStep = bRotatePitch ? FMath::FixedTurn(CurrentRotation.Pitch, TargetRotation.Pitch, RotatingSpeed * DeltaTime) : CurrentRotation.Pitch;
	float YawStep = bRotateYaw ? FMath::FixedTurn(CurrentRotation.Yaw, TargetRotation.Yaw, RotatingSpeed * DeltaTime) : CurrentRotation.Yaw;
	float RollStep = bRotateRoll ? FMath::FixedTurn(CurrentRotation.Roll, TargetRotation.Roll, RotatingSpeed * DeltaTime) : CurrentRotation.Roll;

	FRotator NewRotation = FRotator(PitchStep, YawStep, RollStep);
	GetAvatarActor()->SetActorRotation(NewRotation);

	bool bPitchComplete = !bRotatePitch || FMath::IsNearlyEqual(NewRotation.Pitch, TargetRotation.Pitch, 1.0f);
	bool bYawComplete   = !bRotateYaw   || FMath::IsNearlyEqual(NewRotation.Yaw, TargetRotation.Yaw, 1.0f);
	bool bRollComplete  = !bRotateRoll  || FMath::IsNearlyEqual(NewRotation.Roll, TargetRotation.Roll, 1.0f);
	
	if (GetWorld()->GetTimeSeconds() >= EndTime || (bPitchComplete && bYawComplete && bRollComplete))
	{
		EndTask();
	}
}
