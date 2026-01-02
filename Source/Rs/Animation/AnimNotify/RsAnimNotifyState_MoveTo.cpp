// Copyright 2025 Team BH.


#include "RsAnimNotifyState_MoveTo.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToActorForce.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

void URsAnimNotifyState_MoveTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!PassCondition(Owner))
	{
		return;
	}
	
	// Use current lock on target.
	AActor* LocalTarget = URsBattleLibrary::GetLockOnTarget(Cast<APawn>(Owner));
	// Search new target if current lock on target is not available.
	if (!LocalTarget)
	{
		TArray<AActor*> OutTargets;
		if (URsTargetingLibrary::PerformTargeting(Owner, Owner->GetTransform(), TargetingParams, OutTargets))
		{
			LocalTarget = OutTargets[0];
		}
	}

	if (LocalTarget)
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			if (UGameplayAbility* CurrentAbility = ASC->GetAnimatingAbility())
			{
				UAbilityTask_ApplyRootMotionMoveToActorForce* MoveTask = UAbilityTask_ApplyRootMotionMoveToActorForce::ApplyRootMotionMoveToActorForce(CurrentAbility, NAME_None, LocalTarget, FVector::ZeroVector, ERootMotionMoveToActorTargetOffsetType::AlignFromTargetToSource, TotalDuration, nullptr, nullptr, false, MOVE_Walking, false, nullptr, nullptr, ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity, FVector(), 0.f, false);
				MoveTask->ReadyForActivation();
			}
		}
	}
}
