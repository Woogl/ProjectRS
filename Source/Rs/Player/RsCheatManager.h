// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "RsCheatManager.generated.h"

class ARsPlayerState;

DECLARE_LOG_CATEGORY_EXTERN(LogRsCheat, Log, All);

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
	
	// Useful for testing crash reporting tools
	UFUNCTION(exec)
	void ForceCrash();

	// Useful for identifying hanging pointers that could cause a crash when garbage collection eventually happens
	UFUNCTION(exec)
	void ForceGarbageCollection();

	// Set User ID of local player.
	UFUNCTION(Exec)
	void SetUserId(uint32 NewId);
};
