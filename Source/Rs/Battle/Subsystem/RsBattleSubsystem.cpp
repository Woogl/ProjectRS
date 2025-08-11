// Copyright 2025 Team BH.


#include "RsBattleSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsEnemyCharacter.h"
#include "Rs/Party/RsPartyLibrary.h"

URsBattleSubsystem* URsBattleSubsystem::Get(UObject* WorldContext)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContext, 0))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			return LocalPlayer->GetSubsystem<URsBattleSubsystem>();
		}
	}
	return nullptr;
}

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
	if (!Enemy || URsPartyLibrary::GetAlivePartyMemberCount(this) <= 1)
	{
		AvailableLinkSkillCount = 0;
	}
	else if (LinkSkillType == ERsLinkSkillType::Triple)
	{
		if (!LinkSkillTarget.IsValid())
		{
			AvailableLinkSkillCount = 3;
		}
		else if (LinkSkillTarget == Enemy)
		{
			AvailableLinkSkillCount == FMath::Max(AvailableLinkSkillCount - 1, 0);
		}
		else
		{
			AvailableLinkSkillCount = 3;
		}
	}
	else if (LinkSkillType == ERsLinkSkillType::Parry)
	{
		AvailableLinkSkillCount = 1;
	}
	
	LinkSkillTarget = Enemy;
	OnLinkSkillReady.Broadcast(LinkSkillTarget.Get(), LinkSkillType, AvailableLinkSkillCount);
}

void URsBattleSubsystem::RemoveLinkSkillTarget(ARsEnemyCharacter* Enemy)
{
	if (!Enemy)
	{
		return;
	}
	
	if (LinkSkillTarget == Enemy)
	{
		LinkSkillTarget.Reset();
		AvailableLinkSkillCount = 0;
		OnLinkSkillReady.Broadcast(nullptr, ERsLinkSkillType::None, 0);
	}
}

void URsBattleSubsystem::ResetLinkSkillTarget()
{
	LinkSkillTarget.Reset();
	AvailableLinkSkillCount = 0;

	OnLinkSkillReady.Broadcast(nullptr, ERsLinkSkillType::None, 0);
}

bool URsBattleSubsystem::IsLinkSkillReady() const
{
	return AvailableLinkSkillCount > 0 && LinkSkillTarget.IsValid();
}

void URsBattleSubsystem::DecrementLinkSkillCount()
{
	AvailableLinkSkillCount = FMath::Max(AvailableLinkSkillCount - 1, 0);
	if (AvailableLinkSkillCount == 0)
	{
		OnLinkSkillReady.Broadcast(LinkSkillTarget.Get(), ERsLinkSkillType::None, 0);
	}
}
