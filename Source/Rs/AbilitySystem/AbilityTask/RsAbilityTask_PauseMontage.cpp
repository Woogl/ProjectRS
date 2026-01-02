// Copyright 2024 Team BH.


#include "RsAbilityTask_PauseMontage.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

URsAbilityTask_PauseMontage::URsAbilityTask_PauseMontage()
{
}

URsAbilityTask_PauseMontage* URsAbilityTask_PauseMontage::PauseMontage(UGameplayAbility* OwningAbility, float Duration)
{
	URsAbilityTask_PauseMontage* Task = NewAbilityTask<URsAbilityTask_PauseMontage>(OwningAbility);
	Task->Duration = Duration;
	return Task;
}

void URsAbilityTask_PauseMontage::Activate()
{
	if (Duration <= 0.f)
	{
		EndTask();
		return;
	}
	
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		ASC->CurrentMontageSetPlayRate(KINDA_SMALL_NUMBER);
	}
	
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
	{
		if (AController* Controller = Character->GetController())
		{
			if (Controller->IsLocalPlayerController())
			{
				Controller->SetIgnoreMoveInput(true);
			}
		}
	}
	
	GetAvatarActor()->GetWorldTimerManager().SetTimer(TimerHandle, this, &ThisClass::HandleEndTimer, Duration, false);
}

void URsAbilityTask_PauseMontage::OnDestroy(bool AbilityIsEnding)
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		ASC->CurrentMontageSetPlayRate(1.f);
	}
	
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
	{
		if (AController* Controller = Character->GetController())
		{
			if (Controller->IsLocalPlayerController())
			{
				Controller->SetIgnoreMoveInput(false);
			}
		}
	}

	Super::OnDestroy(AbilityIsEnding);
}

void URsAbilityTask_PauseMontage::HandleEndTimer()
{
	OnFinished.Broadcast();
	EndTask();
}

UAnimInstance* URsAbilityTask_PauseMontage::GetAnimInstance() const
{
	if (AActor* AvatarActor = GetAvatarActor())
	{
		if (ACharacter* Character = Cast<ACharacter>(AvatarActor))
		{
			return Character->GetMesh()->GetAnimInstance();
		}
	}
	return nullptr;
}
