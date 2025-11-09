// Copyright 2024 Team BH.


#include "RsAbilityTask_PauseMontage.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URsAbilityTask_PauseMontage::URsAbilityTask_PauseMontage()
{
	Duration = 0.f;
	MontageSkipTime = 0.f;
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
				if (UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage())
				{
					if (MontageSkipTime > 0.f)
					{
						// Skip frames. (ex. HitStop)
						FMontageBlendSettings BlendSettings;
						BlendSettings.Blend.BlendTime = 0;
						AnimInstance->Montage_PlayWithBlendSettings(CurrentMontage, BlendSettings, 1, EMontagePlayReturnType::MontageLength, 0, false);
					}
					AnimInstance->Montage_Pause();
				}
				
				if (UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(Character->GetMovementComponent()))
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
				if (UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(Character->GetMovementComponent()))
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
