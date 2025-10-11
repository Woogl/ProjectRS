// Copyright 2025 Team BH.


#include "RsAnimNotifyState_TriggerParryLinkSkill.h"

#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Character/RsEnemyCharacter.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotifyState_TriggerParryLinkSkill::URsAnimNotifyState_TriggerParryLinkSkill()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotifyState_TriggerParryLinkSkill::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		TArray<AActor*> ResultActors;
		FTransform WorldTransform = URsTargetingLibrary::GetSocketWorldTransform(MeshComp, SocketName, FTransform(RotationOffset, PositionOffset));
		if (URsTargetingLibrary::PerformTargeting(Owner, WorldTransform, FRsTargetingParams(Shape, Collision, Filter, Sorter), ResultActors))
		{
			if (URsBattleSubsystem* BattleSubsystem = URsBattleSubsystem::Get(MeshComp))
			{
				BattleSubsystem->SetLinkSkillTarget(Cast<ARsEnemyCharacter>(Owner), ERsLinkSkillType::Parry);
			}
		}
	}
}

void URsAnimNotifyState_TriggerParryLinkSkill::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (URsBattleSubsystem* BattleSubsystem = URsBattleSubsystem::Get(MeshComp))
	{
		BattleSubsystem->DecrementLinkSkillCount(Cast<ARsEnemyCharacter>(MeshComp->GetOwner()), ERsLinkSkillType::Parry);
	}
}
