// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "RsUIManagerSubsystem.generated.h"

class URsHUDLayout;
class UCommonActivatableWidget;
/**
 * 
 */
UCLASS()
class RS_API URsUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()

public:
	virtual void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer) override;
	
	URsHUDLayout* GetGameHUD();

private:
	void RegisterGameHUD(UCommonLocalPlayer* LocalPlayer, APawn* Pawn);

	// NOTE: MVVM settings are not saved when this file is set in DefaultGame.ini
	// UPROPERTY(config, EditAnywhere)
	// TSubclassOf<UCommonActivatableWidget> GameHUDClass;

	UPROPERTY(Transient)
	URsHUDLayout* RsHUDInstance = nullptr;
};
