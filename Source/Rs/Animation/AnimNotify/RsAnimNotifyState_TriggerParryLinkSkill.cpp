// Copyright 2025 Team BH.


#include "RsAnimNotifyState_TriggerParryLinkSkill.h"

#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/Character/RsEnemyCharacter.h"

URsAnimNotifyState_TriggerParryLinkSkill::URsAnimNotifyState_TriggerParryLinkSkill()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotifyState_TriggerParryLinkSkill::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (PerformTargeting(MeshComp))
	{
		if (URsBattleSubsystem* BattleSubsystem = MeshComp->GetWorld()->GetSubsystem<URsBattleSubsystem>())
		{
			BattleSubsystem->SetLinkSkillTarget(Cast<ARsEnemyCharacter>(MeshComp->GetOwner()), ERsLinkSkillType::Parry);
		}
		
		// TODO: Gameplay Cue
	}
}

void URsAnimNotifyState_TriggerParryLinkSkill::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (URsBattleSubsystem* BattleSubsystem = MeshComp->GetWorld()->GetSubsystem<URsBattleSubsystem>())
	{
		BattleSubsystem->RemoveLinkSkillTarget(Cast<ARsEnemyCharacter>(MeshComp->GetOwner()));
	}

	// TODO: Gameplay Cue
}
