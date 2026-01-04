// Copyright 2024 Team BH.


#include "RsAnimNotifyState_TurnTo.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Rs/AbilitySystem/AbilityTask/RsAbilityTask_TurnToLocation.h"
#include "Rs/AbilitySystem/Attributes/RsSpeedSet.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

void URsAnimNotifyState_TurnTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	
	// Use current lock on target.
	const AActor* TurnTarget = URsBattleLibrary::GetLockOnTarget(Cast<APawn>(Owner));
	// Search new target if current lock on target is not available.
	if (!TurnTarget)
	{
		TArray<AActor*> OutTargets;
		if (URsTargetingLibrary::PerformTargetingFromComponent(MeshComp, TargetingParams, OutTargets))
		{
			TurnTarget = OutTargets[0];
		}
	}

	if (!TurnTarget)
	{
		return;
	}

	float RotatingSpeed = 300.f;
	if (ACharacter* Character = Cast<ACharacter>(Owner))
	{
		RotatingSpeed = Character->GetCharacterMovement()->RotationRate.Yaw;
	}

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		if (UGameplayAbility* Ability = ASC->GetAnimatingAbility())
		{
			if (float Stat = ASC->GetNumericAttribute(URsSpeedSet::GetMoveSpeedAttribute()))
			{
				RotatingSpeed *= Stat;
			}
			
			FVector TargetLocation = TurnTarget->GetActorLocation();
			URsAbilityTask_TurnToLocation* TurnToTask = URsAbilityTask_TurnToLocation::TurnToLocation(Ability, TargetLocation, RotatingSpeed, TotalDuration);
			TurnToTask->ReadyForActivation();
		}
	}
}
