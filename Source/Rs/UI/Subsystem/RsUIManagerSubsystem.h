// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "RsUIManagerSubsystem.generated.h"

class URsActivatableWidget;
class ARsCharacterBase;
enum class ERsLinkSkillType : uint8;
class URsHUDLayout;
/**
 * 
 */
UCLASS()
class RS_API URsUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()

public:
	virtual void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer) override;
	
	URsHUDLayout* GetGameHUD() const;

private:
	void RegisterGameHUD(UCommonLocalPlayer* LocalPlayer, APawn* Pawn);
	void HandleLinkSkillReady(ARsCharacterBase* Target, ERsLinkSkillType Type, int32 AvailableCount);
	
	UPROPERTY(Transient)
	URsHUDLayout* RsHUDInstance = nullptr;
	
	FDelegateHandle LinkSkillReadyHandle;
};

UCLASS(Config=Game)
class URsUIManagerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override;
	
	UPROPERTY(EditDefaultsOnly, config, Category = "UI")
	TSoftClassPtr<URsActivatableWidget> GameHUDClass;

	UPROPERTY(config, EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<URsActivatableWidget> TripleLinkSkillWidgetClass;
};
