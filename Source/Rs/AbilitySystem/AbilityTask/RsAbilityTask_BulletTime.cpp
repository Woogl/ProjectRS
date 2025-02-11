// Copyright 2024 Team BH.


#include "RsAbilityTask_BulletTime.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

URsAbilityTask_BulletTime::URsAbilityTask_BulletTime()
{
	bTickingTask = true;
	
	TimeDilation = 1.f;
	TimeDilationCurve = nullptr;
	bAffectSelf = true;
	Duration = 0.f;
	ElapsedTime = 0.f;
}

void URsAbilityTask_BulletTime::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(URsAbilityTask_BulletTime, TimeDilation);
	DOREPLIFETIME(URsAbilityTask_BulletTime, TimeDilationCurve);
	DOREPLIFETIME(URsAbilityTask_BulletTime, bAffectSelf);
}

URsAbilityTask_BulletTime* URsAbilityTask_BulletTime::StartBulletTime(UGameplayAbility* OwningAbility, float TimeDilation, float Duration, bool bAffectSelf)
{
	URsAbilityTask_BulletTime* Task = NewAbilityTask<URsAbilityTask_BulletTime>(OwningAbility);
	Task->TimeDilation = TimeDilation;
	Task->bAffectSelf = bAffectSelf;
	Task->Duration = Duration;
	return Task;
}

URsAbilityTask_BulletTime* URsAbilityTask_BulletTime::StartBulletTimeWithCurve(UGameplayAbility* OwningAbility, UCurveFloat* TimeDilationCurve, bool bAffectSelf)
{
	URsAbilityTask_BulletTime* Task = NewAbilityTask<URsAbilityTask_BulletTime>(OwningAbility);
	Task->TimeDilationCurve = TimeDilationCurve;
	Task->bAffectSelf = bAffectSelf;
	return Task;
}

void URsAbilityTask_BulletTime::Activate()
{
	if (TimeDilationCurve)
	{
		float MinTime;
		float MaxTime;
		TimeDilationCurve->GetTimeRange(MinTime, MaxTime);
		Duration = MaxTime;
	}
}

void URsAbilityTask_BulletTime::TickTask(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	if (TimeDilationCurve)
	{
		TimeDilation = TimeDilationCurve->GetFloatValue(ElapsedTime);
	}
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);

	if (bAffectSelf == false)
	{
		// float NewTimeDilation = 1 / TimeDilation;
		GetAvatarActor()->CustomTimeDilation = 1.f / TimeDilation;
	}
	
	if (ElapsedTime >= Duration)
	{
		OnFinished.Broadcast();
		EndTask();
	}
}

void URsAbilityTask_BulletTime::OnDestroy(bool AbilityIsEnding)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	GetAvatarActor()->CustomTimeDilation = 1.f;
	
	Super::OnDestroy(AbilityIsEnding);
}
