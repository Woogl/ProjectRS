// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "RsCheatManager.generated.h"

class ARsPlayerState;
/**
 * 
 */
UCLASS(config = Game, Within = PlayerController, MinimalAPI)
class URsCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	virtual void InitCheatManager() override;
	
private:
	ARsPlayerState* GetRsPlayerState() const;
	
	UFUNCTION(Exec)
	void SetUserId(int32 NewId);
};
