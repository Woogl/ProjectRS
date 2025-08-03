// Copyright 2025 Team BH.


#include "RsBattleSubsystem.h"

#include "Rs/Character/RsEnemyCharacter.h"

ARsEnemyCharacter* URsBattleSubsystem::GetBossInBattle() const
{
	return BossInBattle.Get();
}

void URsBattleSubsystem::SetBossInBattle(ARsEnemyCharacter* Boss)
{
	BossInBattle = Boss;
	OnBossFight.Broadcast(Boss);
}

ARsEnemyCharacter* URsBattleSubsystem::GetLinkSkillTarget() const
{
	return LinkSkillTarget.Get();
}

void URsBattleSubsystem::SetLinkSkillTarget(ARsEnemyCharacter* Enemy, ERsLinkSkillType LinkSkillType)
{
	LinkSkillTarget = Enemy;
	if (LinkSkillTarget.IsValid())
	{
		if (LinkSkillType == ERsLinkSkillType::Parrying)
		{
			LinkSkillCount = 1;
		}
		else if	(LinkSkillType == ERsLinkSkillType::Triple)
		{
			LinkSkillCount = 3;
		}
	}
	OnLinkSkillReady.Broadcast(Enemy, LinkSkillType, LinkSkillCount);
}

bool URsBattleSubsystem::IsLinkSkillReady() const
{
	return LinkSkillCount > 0 && LinkSkillTarget.IsValid();
}

void URsBattleSubsystem::DecrementLinkSkillCount()
{
	LinkSkillCount = FMath::Max(LinkSkillCount - 1, 0);
}
