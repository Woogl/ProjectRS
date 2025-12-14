// Copyright 2024 Team BH.


#include "RsAbilityTask_PauseMontage.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
	TargetMontage = Ability->GetAbilitySystemComponentFromActorInfo()->GetCurrentMontage();
	if (!TargetMontage)
	{
		EndTask();
		return;
	}

	//Ability->GetAbilitySystemComponentFromActorInfo()->CurrentMontageSetPlayRate(SMALL_NUMBER);
	GetAnimInstance()->Montage_Pause(TargetMontage);

	if (ACharacter* Character = Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo()))
	{
		Character->GetCharacterMovement()->DisableMovement();
	}
	
	GetAvatarActor()->GetWorldTimerManager().SetTimer(TimerHandle, this, &ThisClass::HandleEndTimer, Duration, false);
}

void URsAbilityTask_PauseMontage::OnDestroy(bool AbilityIsEnding)
{
	if (TargetMontage)
	{
		//Ability->GetAbilitySystemComponentFromActorInfo()->CurrentMontageSetPlayRate(1.f);
		GetAnimInstance()->Montage_Resume(TargetMontage);
		
		if (ACharacter* Character = Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo()))
		{
			Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
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
			if (USkeletalMeshComponent* Mesh = Character->GetMesh())
			{
				return Mesh->GetAnimInstance();
			}
		}
	}
	return nullptr;
}
