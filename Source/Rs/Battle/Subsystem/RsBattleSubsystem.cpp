// Copyright 2025 Team BH.


#include "RsBattleSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsCharacterBase.h"
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

ARsCharacterBase* URsBattleSubsystem::GetBossInBattle() const
{
	return BossInBattle.Get();
}

void URsBattleSubsystem::SetBossInBattle(ARsCharacterBase* Boss)
{
	BossInBattle = Boss;
	OnBossFight.Broadcast(Boss);
}

void URsBattleSubsystem::SetLinkSkillTarget(ARsCharacterBase* Target, ERsLinkSkillType LinkSkillType)
{
	int32 AlivePartyMemberCount = URsPartyLibrary::GetAlivePartyMemberCount(this);
	if (!Target || AlivePartyMemberCount <= 1)
	{
		FinishLinkSkill();
		return;
	}
	
	if (LinkSkillType == ERsLinkSkillType::Triple)
	{
		AvailableLinkSkillCount = FMath::Min(3, AlivePartyMemberCount);
	}
	else if (LinkSkillType == ERsLinkSkillType::Parry)
	{
		if (AlivePartyMemberCount > 0)
		{
			AvailableLinkSkillCount = 1;
		}
		else
		{
			return;
		}
	}
	
	LinkSkillTarget = Target;
	LastLinkSkillType = LinkSkillType;
	OnLinkSkillReady.Broadcast(LinkSkillTarget.Get(), LastLinkSkillType, AvailableLinkSkillCount);
}

void URsBattleSubsystem::DecrementLinkSkillCount(ARsCharacterBase* Target, ERsLinkSkillType LinkSkillType)
{
	if (!LinkSkillTarget.IsValid() || LinkSkillTarget != Target || LastLinkSkillType != LinkSkillType)
	{
		return;
	}
	
	AvailableLinkSkillCount = FMath::Max(AvailableLinkSkillCount - 1, 0);
	if (AvailableLinkSkillCount > 0)
	{
		OnLinkSkillReady.Broadcast(LinkSkillTarget.Get(), LastLinkSkillType, AvailableLinkSkillCount);
	}
	else
	{
		FinishLinkSkill();
	}
}

void URsBattleSubsystem::FinishLinkSkill()
{
	LinkSkillTarget.Reset();
	AvailableLinkSkillCount = 0;
	OnLinkSkillFinish.Broadcast(LastLinkSkillType);
}

bool URsBattleSubsystem::IsLinkSkillReady() const
{
	return AvailableLinkSkillCount > 0 && LinkSkillTarget.IsValid();
}

ARsCharacterBase* URsBattleSubsystem::GetLinkSkillTarget() const
{
	return LinkSkillTarget.Get();
}

ERsLinkSkillType URsBattleSubsystem::GetLastLinkSkillType() const
{
	return LastLinkSkillType;
}

int32 URsBattleSubsystem::GetAvailableLinkSkillCount() const
{
	return AvailableLinkSkillCount;
}
