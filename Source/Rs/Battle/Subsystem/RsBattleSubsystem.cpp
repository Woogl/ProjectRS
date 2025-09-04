// Copyright 2025 Team BH.


#include "RsBattleSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Rs/Character/RsCharacterBase.h"
#include "Rs/Party/RsPartyLibrary.h"

URsBattleSubsystem* URsBattleSubsystem::Get(const UObject* WorldContext)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull))
		{
			if (ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController())
			{
				return LocalPlayer->GetSubsystem<URsBattleSubsystem>();
			}
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

void URsBattleSubsystem::SetLinkSkillTarget(ARsCharacterBase* Target, ERsLinkSkillType InLinkSkillType)
{
	int32 AliveMemberCount = URsPartyLibrary::GetAlivePartyMemberCount(this);
	if (!Target || AliveMemberCount <= 1)
	{
		FinishLinkSkill();
		return;
	}
	
	switch (InLinkSkillType)
	{
	case ERsLinkSkillType::Triple:
		AvailableLinkSkillCount = FMath::Min(3, AliveMemberCount);
		break;

	case ERsLinkSkillType::Parry:
		// Can't override Parry over Triple
		if (LinkSkillType == ERsLinkSkillType::Triple)
		{
			return;
		}
		AvailableLinkSkillCount = 1;
		break;
		
	default:
		return;
	}
	
	LinkSkillTarget = Target;
	LinkSkillType = InLinkSkillType;
	OnLinkSkillReady.Broadcast(LinkSkillTarget.Get(), LinkSkillType, AvailableLinkSkillCount);
}

void URsBattleSubsystem::DecrementLinkSkillCount(ARsCharacterBase* Target, ERsLinkSkillType InLinkSkillType)
{
	if (!LinkSkillTarget.IsValid() || LinkSkillTarget != Target || LinkSkillType != InLinkSkillType)
	{
		return;
	}
	
	AvailableLinkSkillCount = FMath::Max(AvailableLinkSkillCount - 1, 0);
	if (AvailableLinkSkillCount > 0)
	{
		OnLinkSkillReady.Broadcast(LinkSkillTarget.Get(), LinkSkillType, AvailableLinkSkillCount);
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
	LinkSkillType = ERsLinkSkillType::None;
	OnLinkSkillFinish.Broadcast(LinkSkillType);
}

bool URsBattleSubsystem::IsLinkSkillReady() const
{
	return AvailableLinkSkillCount > 0 && LinkSkillTarget.IsValid();
}

ARsCharacterBase* URsBattleSubsystem::GetLinkSkillTarget() const
{
	return LinkSkillTarget.Get();
}

ERsLinkSkillType URsBattleSubsystem::GetLinkSkillType() const
{
	return LinkSkillType;
}

int32 URsBattleSubsystem::GetAvailableLinkSkillCount() const
{
	return AvailableLinkSkillCount;
}
