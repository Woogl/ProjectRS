// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "RsUIManagerSubsystem.generated.h"

class ARsCharacterBase;
enum class ERsLinkSkillType : uint8;
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
	
	URsHUDLayout* GetGameHUD() const;

private:
	void RegisterGameHUD(UCommonLocalPlayer* LocalPlayer, APawn* Pawn);
	void HandleLinkSkillReady(ARsCharacterBase* Target, ERsLinkSkillType Type, int32 AvailableCount);

	UPROPERTY(Transient)
	URsHUDLayout* RsHUDInstance = nullptr;
	
	FDelegateHandle LinkSkillReadyHandle;
};
