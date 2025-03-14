// Copyright 2024 Team BH.


#include "RsAbilityTask_PauseMontage.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URsAbilityTask_PauseMontage::URsAbilityTask_PauseMontage()
{
	Duration = 0.f;
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
	
	if (AActor* AvatarActor = GetAvatarActor())
	{
		if (ACharacter* Character = Cast<ACharacter>(AvatarActor))
		{
			if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Pause();
				UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
				if (CharMoveComp)
				{
					CharMoveComp->DisableMovement();
				}
				AvatarActor->GetWorldTimerManager().SetTimer(Timer, this, &ThisClass::HandleEndTimer, Duration, false);
			}
		}
	}
}

void URsAbilityTask_PauseMontage::OnDestroy(bool AbilityIsEnding)
{
	if (AActor* AvatarActor = GetAvatarActor())
	{
		if (ACharacter* Character = Cast<ACharacter>(AvatarActor))
		{
			if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Resume(nullptr);
				UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
				if (CharMoveComp)
				{
					CharMoveComp->SetMovementMode(MOVE_Falling);
				}
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
