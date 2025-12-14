// Copyright 2024 Team BH.


#include "RsAbilityTask_PauseMontage.h"

URsAbilityTask_PauseMontage::URsAbilityTask_PauseMontage()
{
}

URsAbilityTask_PauseMontage* URsAbilityTask_PauseMontage::PauseMontage(UGameplayAbility* OwningAbility, float Duration, float MontageSkipTime)
{
	URsAbilityTask_PauseMontage* Task = NewAbilityTask<URsAbilityTask_PauseMontage>(OwningAbility);
	Task->Duration = Duration;
	Task->MontageSkipTime = MontageSkipTime;
	return Task;
}

void URsAbilityTask_PauseMontage::Activate()
{
	AActor* AvatarActor = GetAvatarActor();
	if (Duration <= 0.f || !AvatarActor)
	{
		EndTask();
		return;
	}
	
	// TODO: Pause montage only.
	// TODO: Skip montage time.
	AvatarActor->CustomTimeDilation = 0.f;
	AvatarActor->GetWorldTimerManager().SetTimer(Timer, this, &ThisClass::HandleEndTimer, Duration, false);
}

void URsAbilityTask_PauseMontage::OnDestroy(bool AbilityIsEnding)
{
	if (AActor* AvatarActor = GetAvatarActor())
	{
		AvatarActor->CustomTimeDilation = 1.f;
	}

	Super::OnDestroy(AbilityIsEnding);
}

void URsAbilityTask_PauseMontage::HandleEndTimer()
{
	OnFinished.Broadcast();
	EndTask();
}
