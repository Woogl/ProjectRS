// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "RsBattleSubsystem.generated.h"

class ARsCharacterBase;

UENUM(BlueprintType)
enum class ERsLinkSkillType : uint8
{
	Parry,
	Triple,
};

/**
 * Subsystem for managing battle information.
 */
UCLASS()
class RS_API URsBattleSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	static URsBattleSubsystem* Get(const UObject* WorldContext);
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossFight, ARsCharacterBase*);
	FOnBossFight OnBossFight;
	
	ARsCharacterBase* GetBossInBattle() const;
	void SetBossInBattle(ARsCharacterBase* Boss);

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnLinkSkillReady, ARsCharacterBase*, ERsLinkSkillType, int32);
	FOnLinkSkillReady OnLinkSkillReady;
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLinkSkillFinish, ERsLinkSkillType);
	FOnLinkSkillFinish OnLinkSkillFinish;
	
	void SetLinkSkillTarget(ARsCharacterBase* Target, ERsLinkSkillType LinkSkillType);
	void DecrementLinkSkillCount(ARsCharacterBase* Target, ERsLinkSkillType LinkSkillType);
	void FinishLinkSkill();
	
	bool IsLinkSkillReady() const;
	ARsCharacterBase* GetLinkSkillTarget() const;
	ERsLinkSkillType GetLastLinkSkillType() const;
	int32 GetAvailableLinkSkillCount() const;

private:
	TWeakObjectPtr<ARsCharacterBase> BossInBattle;	
	TWeakObjectPtr<ARsCharacterBase> LinkSkillTarget;
	int32 AvailableLinkSkillCount = 0;
	ERsLinkSkillType LastLinkSkillType = ERsLinkSkillType::Parry;
};
