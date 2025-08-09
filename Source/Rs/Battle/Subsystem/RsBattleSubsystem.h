// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "RsBattleSubsystem.generated.h"

class ARsEnemyCharacter;

UENUM(BlueprintType)
enum class ERsLinkSkillType : uint8
{
	None,
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
	static URsBattleSubsystem* Get(UObject* WorldContext);
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossFight, ARsEnemyCharacter*);
	FOnBossFight OnBossFight;
	
	ARsEnemyCharacter* GetBossInBattle() const;
	void SetBossInBattle(ARsEnemyCharacter* Boss);

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnLinkSkillReady, ARsEnemyCharacter*, ERsLinkSkillType, int32);
	FOnLinkSkillReady OnLinkSkillReady;

	ARsEnemyCharacter* GetLinkSkillTarget() const;
	void SetLinkSkillTarget(ARsEnemyCharacter* Enemy, ERsLinkSkillType LinkSkillType);
	void RemoveLinkSkillTarget(ARsEnemyCharacter* Enemy);
	void ResetLinkSkillTarget();

	bool IsLinkSkillReady() const;
	void DecrementLinkSkillCount();

private:
	TWeakObjectPtr<ARsEnemyCharacter> BossInBattle;	
	TWeakObjectPtr<ARsEnemyCharacter> LinkSkillTarget;
	int32 AvailableLinkSkillCount = 0;
};
