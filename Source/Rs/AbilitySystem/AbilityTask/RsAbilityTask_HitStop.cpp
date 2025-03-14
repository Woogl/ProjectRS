// Copyright 2024 Team BH.


#include "RsAbilityTask_HitStop.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URsAbilityTask_HitStop::URsAbilityTask_HitStop()
{
	Duration = 0.f;
}

URsAbilityTask_HitStop* URsAbilityTask_HitStop::StartHitStop(UGameplayAbility* OwningAbility, float Duration)
{
	URsAbilityTask_HitStop* Task = NewAbilityTask<URsAbilityTask_HitStop>(OwningAbility);
	Task->Duration = Duration;
	return Task;
}

void URsAbilityTask_HitStop::Activate()
{
	if (AActor* AvatarActor = GetAvatarActor())
	{
		if (ACharacter* Character = Cast<ACharacter>(AvatarActor))
		{
			if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Pause();
				AvatarActor->GetWorldTimerManager().SetTimer(Timer, this, &ThisClass::HandleEndTimer, Duration, false);
			}
		}
	}
}

void URsAbilityTask_HitStop::OnDestroy(bool AbilityIsEnding)
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

void URsAbilityTask_HitStop::HandleEndTimer()
{
	if (AActor* AvatarActor = GetAvatarActor())
	{
		if (ACharacter* Character = Cast<ACharacter>(AvatarActor))
		{
			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
			if (CharMoveComp)
			{
				CharMoveComp->DisableMovement();
			}
		}
	}
	
	OnFinished.Broadcast();
	EndTask();
}
