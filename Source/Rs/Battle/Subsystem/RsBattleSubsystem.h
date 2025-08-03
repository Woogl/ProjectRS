// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RsBattleSubsystem.generated.h"

class ARsEnemyCharacter;

UENUM(BlueprintType)
enum class ERsLinkSkillType : uint8
{
	Parrying,
	Triple,
};

/**
 * Subsystem for managing combat information.
 */
UCLASS()
class RS_API URsBattleSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossFight, ARsEnemyCharacter*);
	FOnBossFight OnBossFight;
	
	ARsEnemyCharacter* GetBossInBattle() const;
	void SetBossInBattle(ARsEnemyCharacter* Boss);

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnLinkSkillReady, ARsEnemyCharacter*, ERsLinkSkillType, int32);
	FOnLinkSkillReady OnLinkSkillReady;

	ARsEnemyCharacter* GetLinkSkillTarget() const;
	void SetLinkSkillTarget(ARsEnemyCharacter* Enemy, ERsLinkSkillType LinkSkillType);

	bool IsLinkSkillReady() const;
	void DecrementLinkSkillCount();

private:
	TWeakObjectPtr<ARsEnemyCharacter> BossInBattle;
	TWeakObjectPtr<ARsEnemyCharacter> LinkSkillTarget;

	int32 LinkSkillCount = 0;
};
